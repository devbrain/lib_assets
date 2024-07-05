//
// Created by igor on 7/5/24.
//

#include <map>
#include <algorithm>
#include "world_loader.hh"

#include <bsw/override.hh>
#include <bsw/exception.hh>

namespace neutrino::tiled {
	using image2texture_t = std::map <assets::image_id_t, tiles_texture_id_t>;

	static image2texture_t create_image_mapping(assets::world& world) {
		std::map <assets::image_id_t, tiles_texture_id_t> out;
		tiles_texture_id_t tex_id(0);
		for (const auto& itr : world.get_images()) {
			out.insert(std::make_pair(itr.first, tex_id++));
		}
		return out;
	}

	using tileset_2_texid_t = std::map <std::size_t, tiles_texture_id_t>;

	static std::tuple <atlas_builder, tileset_2_texid_t> create_atlas_builder(assets::world& world,
	                                                                          const image2texture_t& image2texture) {
		// create correct ordering of surfaces
		std::vector <std::pair <assets::image_id_t, tiles_texture_id_t>> ordering;
		for (const auto& itr : image2texture) {
			ordering.emplace_back(itr);
		}
		std::sort(ordering.begin(), ordering.end(), [](const auto& a, const auto& b) {
			return a.second < b.second;
		});
		atlas_builder out;
		tileset_2_texid_t tileset_mapping;

		for (const auto& [image_id, tex_id] : ordering) {
			bool found = false;
			// first, check for images in tilesets
			std::size_t tsnum = 0;
			for (const auto& ts : world.get_tilesets()) {
				if (ts.get_image() == image_id) {
					auto itr = world.get_images().find(image_id);
					ENFORCE(itr != world.get_images().end());
					out.m_material.emplace_back(tex_id, ts.get_rects(), std::move(itr->second));
					found = true;
					tileset_mapping.insert(std::make_pair(tsnum, tex_id));
					break;
				}
				tsnum++;
			}
			if (!found) {
				// if no image was found in tileset, check for images in image_layers
				world.visit_layers([&found, &world, &out, image_id, tex_id](const auto& layer) {
					if (!found) {
						std::visit(bsw::overload(
							           [&found, &world, &out, image_id, tex_id
							           ](const assets::image_layer& image_layer) {
								           if (image_layer.get_image_id() == image_id) {
									           found = true;
									           auto itr = world.get_images().find(image_id);
									           ENFORCE(itr != world.get_images().end());
									           out.m_material.emplace_back(tex_id, std::move(itr->second));
								           }
							           },
							           [](const auto&) {
							           }
						           ), layer);
					}
				});
			}
		}
		return {std::move(out), std::move(tileset_mapping)};
	}

	using ts_info_t = std::pair <unsigned, std::size_t>; // first_gid, index
	using ts_info_vec_t = std::vector <ts_info_t>;

	static tile from_global_id(assets::tile_id_t tid, const tileset_2_texid_t& tileset_mapping,
							   const ts_info_vec_t& ts_info, std::size_t empty) {
		auto global_id = tid.value_of();
		if (global_id == empty) {
			return tile(tile_id_t(EMPTY_TILE_VALUE));
		}
		auto lower_bound = std::lower_bound(ts_info.begin(), ts_info.end(), global_id,
																	   [](const auto& a, const auto& b) {
																		   return a.first < b;
																	   });
		auto tset = std::max_element(ts_info.begin(), lower_bound);
		ENFORCE(tset != ts_info.end());
		ENFORCE(global_id >= tset->first);
		auto local_id = tile_id_t(global_id - tset->first);
		auto tx_itr = tileset_mapping.find(tset->second);
		ENFORCE(tx_itr != tileset_mapping.end());
		auto tex_id = tx_itr->second;
		return {tex_id, local_id};
	}

	static tile from_tile_descr_id(const assets::tile_description& td,
	                           const assets::world& world,
	                           const tileset_2_texid_t& tileset_mapping,
	                           const ts_info_vec_t& ts_info,
	                           std::size_t empty) {
		auto tl = from_global_id(td.tile_id, tileset_mapping, ts_info, empty);
		if (tiles_layer::is_empty(tl)) {
			return tl;
		}
		tl.info.flags.animated = world.is_inimated(td.tile_id) ? 1 : 0;
		tl.info.flags.hor_flip = td.flip.contains(assets::flip_t::HORIZONTAL) ? 1 : 0;
		tl.info.flags.vert_flip = td.flip.contains(assets::flip_t::VERTICAL) ? 1 : 0;
		tl.info.flags.diag_flip = td.flip.contains(assets::flip_t::DIAGONAL) ? 1 : 0;

		return tl;
	}

	world_model create_world_model(const assets::world& world,
	                               const tileset_2_texid_t& tileset_mapping,
	                               const image2texture_t& image2texture) {
		ts_info_vec_t ts_info;
		const auto& tilesets = world.get_tilesets();
		for (std::size_t i = 0; i < tilesets.size(); i++) {
			ts_info.emplace_back(tilesets[i].get_first_gid(), i);
		}
		std::sort(ts_info.begin(), ts_info.end(), [](const auto& a, const auto& b) {
			return a.first < b.first;
		});
		world_model model;

		world.visit_layers([&model, &image2texture, &tileset_mapping, &world, &ts_info](const auto& layer) {
			std::visit(bsw::overload(
				           [&model, &image2texture, &tileset_mapping](const assets::image_layer& img_layer) {
					           const auto image_id = img_layer.get_image_id();
					           const auto itr = image2texture.find(image_id);
					           ENFORCE(itr != image2texture.end());
					           model.prepend(image_layer(itr->second));
				           },
				           [&model, &image2texture, &tileset_mapping, &world, &ts_info](
				           const assets::tiles_layer& tls_layer) {
					           const auto w_in_tiles = tile_coord_t(tls_layer.width());
					           const auto h_in_tiles = tile_coord_t(tls_layer.height());
					           const auto tile_dims_in_px = world.get_tiles_dimension();
					           const auto empty = world.get_empty_tile_id().value_of();
					           tiles_layer out(w_in_tiles,
					                           h_in_tiles,
					                           world_coords_t(tile_dims_in_px.w),
					                           world_coords_t(tile_dims_in_px.h));
					           for (unsigned int x = 0; x < tls_layer.width(); x++) {
						           for (unsigned int y = 0; y < tls_layer.height(); y++) {
							           const auto td = tls_layer.at(x, y);
							           const tile_coord_t tx(x);
							           const tile_coord_t ty(y);
							           out.at(tx, ty) = from_tile_descr_id(td, world, tileset_mapping, ts_info, empty);
						           }
					           }
					           model.prepend(out);
				           },
				           []([[maybe_unused]] const auto& ignore_obj_layer) {
				           }
			           ), layer);
		});
		// assign animations
		const auto empty = world.get_empty_tile_id().value_of();
		for (const auto& [gid, ani_seq] : world.get_animations()) {
			auto base_tile = from_global_id(gid, tileset_mapping, ts_info, empty);
			for (const auto& f : ani_seq) {
				model.add_animation(base_tile, from_global_id(f.m_tileid, tileset_mapping, ts_info, empty), f.m_duration);
			}
		}
		// first layer is bg layer
		model.prepend(color_layer(world.get_background_color()));
		return model;
	}

	std::tuple <atlas_builder, world_model> world_loader(assets::world&& w) {
		assets::world world(std::move(w));
		auto image2texture = create_image_mapping(world);
		auto [atlas, tileset_mapping] = create_atlas_builder(world, image2texture);
		auto model = create_world_model(world, tileset_mapping, image2texture);
		return {std::move(atlas), std::move(model)};
	}
}
