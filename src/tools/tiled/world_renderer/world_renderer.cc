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

	world_renderer::world_renderer(sdl::renderer& renderer, sdl::area_type& dimensions,
	                               sdl::pixel_format pixel_format)
		: m_renderer(renderer),
		  m_dimension(dimensions),
		  m_target(m_renderer, pixel_format, dimensions.w, dimensions.h, sdl::texture::access::TARGET),
		  m_model(nullptr),
		  m_last_update_time(0) {
	}

	void world_renderer::init(atlas_builder&& atlas_builder_, const world_model& model) {
		ENFORCE(m_model == nullptr);
		ENFORCE(m_animations.empty())
		m_model = &model;
		for (auto& material : atlas_builder_.m_material) {
			m_texture_atlas.add(sdl::texture(m_renderer, material.m_image), std::move(material.m_coords));
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

	void world_renderer::_update() {
		if (!m_model) {
			return;
		}
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
					[this, delta_time](const tiles_layer& tl) {
						draw_layer(tl, delta_time);
					}
				),
				layer
			);
		}
	}

	void world_renderer::draw_layer(const image_layer& layer) {
	}

	void world_renderer::draw_layer(const color_layer& layer) const {
		const auto c = m_renderer.get_active_color();
		m_renderer.set_active_color(layer.get_color());
		m_renderer.clear();
		m_renderer.set_active_color(c);
	}

	void world_renderer::draw_layer(const tiles_layer& layer, std::chrono::milliseconds delta_time) {
	}

	world_renderer::animation_data::animation_data(const animation_sequence& seq)
		: m_current_frame(0),
		  m_time_in_current_frame(0),
		  m_sequence(seq) {
	}
}
