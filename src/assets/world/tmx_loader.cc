//
// Created by igor on 7/2/24.
//

#include <vector>
#include <assets/resources/world/world.hh>

#include <bsw/override.hh>
#include <bsw/exception.hh>
#include <bsw/logger/logger.hh>
#include <bsw/io/memory_stream_buf.hh>
#include <bsw/mp/type_name/type_name.hpp>
#include <utility>

#include "tmx/xml_reader.hh"
#include "tmx/json_reader.hh"
#include "tmx/color.hh"
#include "tmx/map.hh"

#include "tmx_loader.hh"

#include "assets/resources/world/world_image_layer.hh"

namespace neutrino::assets::tmx {
	static map load_map(const char* data, std::size_t length, path_resolver_t resolver) {
		const auto doc_type = reader::guess_document_type(data, length);
		switch (doc_type) {
			case reader::XML_DOCUMENT:
				return map::parse(xml_reader::load(data, length, "map"), std::move(resolver));
			case reader::JSON_DOCUMENT:
				return map::parse(json_reader::load(data, length, nullptr), std::move(resolver));
			default:
				RAISE_EX("Unknown document type");
		}
	}

	static sdl::surface create_surface(std::istream& is, const image& img,
	                                   const data_loader <sdl::surface, image_resource>& image_loader) {
		auto s = image_loader.load(is);
		if (img.transparent()) {
			colori c(*img.transparent());
			s.color_key(sdl::color(c.r, c.g, c.b, c.a));
		}
		return s;
	}

	static sdl::surface create_surface(const image& img, const path_resolver_t& resolver,
	                                   const data_loader <sdl::surface, image_resource>& image_loader) {
		if (!img.data().empty()) {
			bsw::io::memory_input_stream is(img.data().data(), img.data().size());
			return create_surface(is, img, image_loader);
		}
		const auto content = resolver(img.source());
		bsw::io::memory_input_stream is(content.c_str(), content.size());
		return create_surface(is, img, image_loader);
	}

	static void load_images(world& out, const map& in, const path_resolver_t& resolver,
	                        const data_loader <sdl::surface, image_resource>& image_loader) {
		for (const auto& layer : in.layers()) {
			std::visit(bsw::overload(
				           [&out, &resolver, &image_loader](const image_layer& l) {
					           if (const image* img = l.get_image()) {
						           image_id_t image_id(std::hash <image>{}(*img));
						           out.add_image(image_id, create_surface(*img, resolver, image_loader));
					           }
				           },
				           [](const auto&) {
				           }
			           ), layer);
		}
		for (const auto& ts : in.tile_sets()) {
			if (const image* img = ts.get_image()) {
				image_id_t image_id(std::hash <image>{}(*img));
				out.add_image(image_id, create_surface(*img, resolver, image_loader));
			}

			for (const auto& t : ts) {
				if (t.get_image()) {
					EVLOG_TRACE(EVLOG_WARNING, "Per tile images are not supported");
					break;
				}
			}
		}
	}

	static void load_tilesets(world& out, const map& in) {
		for (const auto& ts : in.tile_sets()) {
			if (const image* img = ts.get_image()) {
				const auto tiles_count = ts.tile_count();
				if (tiles_count == 0) {
					EVLOG_TRACE(EVLOG_WARNING, "Found tileset with zero tiles");
					continue;
				}
				const image_id_t image_id(std::hash <image>{}(*img));
				const auto first_gid = ts.first_gid();
				const auto tw = ts.tile_width();
				const auto tl = ts.tile_height();
				tiles_set ots(image_id, first_gid,
					sdl::area_type(static_cast<int>(tw), static_cast<int>(tl)),
					tiles_count);
				ts.assign(ots);
				sdl::area_type image_dims;
				if (auto isize = img->size()) {
					image_dims = *isize;
				} else {
					image_dims = out.get_image(image_id).get_dimanesions();
				}

				for (unsigned i=0; i<tiles_count; i++) {
					const local_tile_id_t local_id(i);
					ots.set_rect(local_id, ts.get_coords(i, image_dims));
					if (auto* local_tile = ts.get_tile(i)) {
						local_tile->assign(ots.get_property(local_id));
						const auto& local_anim = local_tile->get_animation();
						if (const auto& frames = local_anim.frames(); !frames.empty()) {
							const auto global_tile_id = ots.from_local(local_id);
							animation_sequence as;
							for (const auto& f : frames) {
								const local_tile_id_t lid(f.id());
								const auto tid = ots.from_local(lid);
								as.add(tid, f.duration());
							}
							if (!as.empty()) {
								out.add_animation_sequence(global_tile_id, as);
							}
						}
					}
				}
				out.add_tile_set(ots);
			} else {
				EVLOG_TRACE(EVLOG_WARNING, "Found tileset without image");
			}
		}
	}

	static void load_layers(world& out, const map& in) {
		for (const auto& ol : in.objects()) {
			assets::object_layer result;
			ol.assign(result);
			for (const auto& tmx_obj : ol.objects()) {
				result.add(transform(tmx_obj));
			}
			out.add_layer(result);
		}
		for (const auto& tmx_layer : in.layers()) {
			std::visit(bsw::overload(
				[&out](const image_layer& tmx_image_layer) {
					if (tmx_image_layer.visible()) {
						if (const auto* img = tmx_image_layer.get_image()) {
							const image_id_t image_id(std::hash <image>{}(*img));
							const auto offs_x = tmx_image_layer.offset_x();
							const auto offs_y = tmx_image_layer.offset_y();
							assets::image_layer result(sdl::point(offs_x, offs_y), image_id);
							out.add_layer(result);
						}
					}
				},
				[&out](const tile_layer& tmx_tile_layer) {
					if (tmx_tile_layer.visible()) {
						tiles_layer result(tmx_tile_layer.width(), tmx_tile_layer.height(), out.get_empty_tile_id());
						tmx_tile_layer.assign(result);
						unsigned x = 0;
						unsigned y = 0;
						for (const auto& c : tmx_tile_layer.cells()) {
							flip_t flp{};
							if (c.diag_flipped()) {
								flp |= flip_t::DIAGONAL;
							}
							if (c.hor_flipped()) {
								flp |= flip_t::HORIZONTAL;
							}
							if (c.vert_flipped()) {
								flp |= flip_t::VERTICAL;
							}

							result.at(x, y) = tile_description(tile_id_t(c.gid()), flp);
							x++;
							if (x >= tmx_tile_layer.width()) {
								x = 0;
								y++;
							}
						}
						out.add_layer(result);
					}
				}
				), tmx_layer);
		}
	}

	static world load(const char* text, std::size_t size, const path_resolver_t& resolver,
	                  const data_loader <sdl::surface, image_resource>& image_loader) {
		const auto raw = load_map(text, size, resolver);
		auto icolor = raw.background_color();

		world w(raw.orientation(),
		        raw.render_order(),
		        sdl::area_type{static_cast <int>(raw.width()), static_cast <int>(raw.height())},
		        sdl::area_type{static_cast <int>(raw.tile_width()), static_cast <int>(raw.tile_height())},
		        sdl::color{icolor.r, icolor.g, icolor.b, icolor.a},
		        raw.hex_side_length(),
		        raw.stagger_axis(),
		        raw.stagger_index(),
		        raw.infinite());
		w.set_empty_tile_id(tile_id_t(0));
		raw.assign(w);
		load_images(w, raw, resolver, image_loader);
		load_tilesets(w, raw);
		load_layers(w, raw);
		return w;
	}

	world load(std::istream& is, const path_resolver_t& resolver, const data_loader <sdl::surface, image_resource>& image_loader) {
		auto txt = std::vector <char>((std::istreambuf_iterator <char>(is)),
		                              std::istreambuf_iterator <char>());
		return load(txt.data(), txt.size(), resolver, image_loader);
	}
}
