//
// Created by igor on 7/4/24.
//


#include "world_renderer.hh"
#include "tile.hh"
#include <bsw/exception.hh>
#include <bsw/override.hh>
#include <sdlpp/system/timers.hh>

namespace neutrino::tiled {
	namespace detail {
		struct render_ctx {
			sdl::renderer& m_renderer;
			std::optional <sdl::texture> m_target;

			explicit render_ctx(sdl::renderer& r)
				: m_renderer(r),
				  m_target(r.get_target()) {
			}

			~render_ctx() {
				if (m_target) {
					m_renderer.set_target(*m_target);
				} else {
					m_renderer.restore_default_target();
				}
			}
		};
	}

	world_renderer::world_renderer(sdl::renderer& renderer,
	                               const sdl::area_type& dimensions,
	                               sdl::pixel_format pixel_format)
		: m_renderer(renderer),
		  m_dimension(dimensions),
		  m_target(m_renderer, pixel_format, dimensions.w, dimensions.h, sdl::texture::access::TARGET),
		  m_model(nullptr),
		  m_last_update_time(0),
		  m_camera_x(0),
		  m_camera_y(0) {
	}

	world_renderer::~world_renderer() {
		deinit();
	}

	void world_renderer::init(atlas_builder&& atlas_builder_, const world_model& model) {
		ENFORCE(m_model == nullptr);
		ENFORCE(m_animations.empty())
		m_model = &model;
		for (auto& material : atlas_builder_.m_material) {
			if (!material.m_coords.empty()) {
				m_texture_atlas.add(sdl::texture(m_renderer, material.m_image), std::move(material.m_coords));
			} else {
				sdl::rect r(material.m_image.get_dimanesions());
				std::vector <sdl::rect> coords;
				coords.push_back(r);
				m_texture_atlas.add(sdl::texture(m_renderer, material.m_image), std::move(coords));
			}
		}
		for (const auto& [tl, seq] : model.get_animations()) {
			m_animations.emplace(tl, seq);
		}
	}

	void world_renderer::deinit() {
		m_texture_atlas.clear();
		m_model = nullptr;
		m_animations.clear();
	}

	void world_renderer::update() {
		detail::render_ctx render_ctx(m_renderer);
		m_renderer.set_target(m_target);
		_update();
	}

	void world_renderer::present(const sdl::rect& dst_rect) const {
		const sdl::rect src_rect(m_dimension);
		m_renderer.copy(m_target, src_rect, dst_rect);
	}

	void world_renderer::set_camera(int& x, int& y) {
		ENFORCE(m_model);
		if (x > 0 && x + m_dimension.w < m_model->get_world_width().value_of()) {
			m_camera_x = world_coords_t(x);
		}
		if (y > 0 && y + m_dimension.h < m_model->get_world_height().value_of()) {
			m_camera_y = world_coords_t(y);
		}
		x = m_camera_x.value_of();
		y = m_camera_y.value_of();
	}

	void world_renderer::_update() {
		if (!m_model) {
			return;
		}
		std::set<tile> updated_animations;
		auto delta_time = m_last_update_time.count() == 0
			                  ? std::chrono::milliseconds(0)
			                  : sdl::get_ms_since_init() - m_last_update_time;
		m_last_update_time = sdl::get_ms_since_init();
		for (const auto& layer : *m_model) {
			std::visit(
				bsw::overload(
					[this](const image_layer& il) {
						draw_layer(il);
					},
					[this](const color_layer& cl) {
						draw_layer(cl);
					},
					[this, delta_time, &updated_animations](const tiles_layer& tl) {
						draw_layer(tl, delta_time, updated_animations);
					}
				),
				layer
			);
		}
	}

	void world_renderer::draw_layer(const image_layer& layer) {
		auto [tex_ptr, dims] = m_texture_atlas.get(layer.get_texture_id());
		ENFORCE(tex_ptr != nullptr);

		unsigned int world_w = m_model->get_world_width().value_of();
		unsigned int world_h = m_model->get_world_height().value_of();
		unsigned int layer_w_px = dims.w;
		unsigned int layer_h_px = dims.h;
		unsigned int x1 = m_camera_x.value_of();
		unsigned int y1 = m_camera_y.value_of();

		float relative_x = static_cast<float>(x1) / static_cast<float>(world_w);
		float relative_y = static_cast<float>(y1) / static_cast<float>(world_h);

		int img_x = static_cast<int> (relative_x*dims.w);
		int img_y = static_cast<int> (relative_y*dims.h);

		int image_w = std::min(m_dimension.w, layer_w_px - img_x);
		int image_h = std::min(m_dimension.h, layer_h_px - img_y);

		sdl::rect src_rect(img_x, img_y, image_w, image_h);
		sdl::rect dst_rect(0, 0, image_w, image_h);

		m_renderer.copy(*tex_ptr, src_rect, dst_rect);

	}

	void world_renderer::draw_layer(const color_layer& layer) const {
		const auto c = m_renderer.get_active_color();
		m_renderer.set_active_color(layer.get_color());
		m_renderer.clear();
		m_renderer.set_active_color(c);
	}

	void world_renderer::draw_layer(const tiles_layer& layer, std::chrono::milliseconds delta_time,
									std::set<tile>& updated_animations) {
		const unsigned int tile_w = layer.get_tile_width().value_of();
		const unsigned int tile_h = layer.get_tile_height().value_of();
		const unsigned int layer_w = layer.get_width().value_of();
		const unsigned int layer_h = layer.get_height().value_of();
		const unsigned int layer_w_px = tile_w*layer_w;
		const unsigned int layer_h_px = tile_h*layer_h;

		const unsigned int x1 = m_camera_x.value_of();
		const unsigned int y1 = m_camera_y.value_of();

		const unsigned int viewport_w = std::min(m_dimension.w, layer_w_px - x1);
		const unsigned int viewport_h = std::min(m_dimension.h, layer_h_px - y1);

		const unsigned int x2 = x1 + viewport_w;
		const unsigned int y2 = y1 + viewport_h;

		const unsigned int left_column = x1 / tile_w;
		const unsigned int offset_left = x1 % tile_w;

		const unsigned int top_row = y1 / tile_h;
		const unsigned int offset_top = y1 % tile_h;

		unsigned int right_column = x2 / tile_w;
		const unsigned int offset_right = x2 - right_column*tile_w;

		unsigned int botton_row = y2 / tile_h;
		const unsigned int offset_bottom = y2 - botton_row*tile_h;

		for (unsigned int x=left_column; x<=right_column; x++) {
			for (unsigned int y=top_row; y<=botton_row; y++) {

				const tile_coord_t tx = x < layer_w ? tile_coord_t(x) : tile_coord_t(x - 1);
				const tile_coord_t ty = y < layer_h ? tile_coord_t(y) : tile_coord_t(y - 1);

				auto cell = layer.at(tx, ty);
				if (tiles_layer::is_empty(cell)) {
					continue;
				}
				if (cell.info.flags.animated) {
					auto itr = m_animations.find(cell);
					ENFORCE(itr != m_animations.end());
					auto current_frame = itr->second.m_current_frame;
					const auto& frames = itr->second.m_sequence.get_frames();
					if (updated_animations.find(cell) == updated_animations.end()) {
						updated_animations.insert(cell);
						if (itr->second.m_time_in_current_frame > frames[current_frame].m_duration) {
							current_frame++;
							if (current_frame >= frames.size()) {
								current_frame = 0;
							}
							itr->second.m_time_in_current_frame = std::chrono::milliseconds(0);
							itr->second.m_current_frame = current_frame;
						} else {
							itr->second.m_time_in_current_frame += delta_time;
						}
					}
					cell = frames[current_frame].m_tile;
				}
				auto [tex_ptr, src_rect] = m_texture_atlas.get(cell);
				ENFORCE(tex_ptr);

				sdl::rect dst_rect;
				if (x == left_column) {
					dst_rect.x = 0;
				} else {
					dst_rect.x = static_cast <int>((tile_w - offset_left) + (x - left_column - 1)*tile_w);
				}
				if (y == top_row) {
					dst_rect.y = 0;
				} else {
					dst_rect.y =  static_cast <int>((tile_h - offset_top) + (y - top_row - 1)*tile_h);
				}

				if (x == left_column) {
					src_rect.x += static_cast<int>(offset_left);
					src_rect.w -= static_cast<int>(offset_left);
				} else if (x == right_column) {
					src_rect.w = static_cast<int>(offset_right);
				}
				if (y == top_row) {
					src_rect.y += static_cast<int>(offset_top);
					src_rect.h -= static_cast<int>(offset_top);
				} else if (y == botton_row) {
					src_rect.h = static_cast<int>(offset_bottom);
				}
				dst_rect.w = src_rect.w;
				dst_rect.h = src_rect.h;
				// TODO add flip
				m_renderer.copy(*tex_ptr, src_rect, dst_rect);
			}
		}
	}

	world_renderer::animation_data::animation_data(const animation_sequence& seq)
		: m_current_frame(0),
		  m_time_in_current_frame(0),
		  m_sequence(seq) {
	}
}
