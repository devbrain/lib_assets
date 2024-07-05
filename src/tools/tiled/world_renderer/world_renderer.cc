//
// Created by igor on 7/4/24.
//

#include "world_renderer.hh"
#include "tile.hh"
namespace neutrino::tiled {

	namespace detail {
		struct render_ctx {
			sdl::renderer& m_renderer;
			std::optional<sdl::texture> m_target;
			explicit render_ctx(sdl::renderer& r)
				: m_renderer(r),
				  m_target(r.get_target()) {}
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
		  m_target(m_renderer, pixel_format, dimensions.w, dimensions.h, sdl::texture::access::TARGET) {
	}

	void world_renderer::init(const assets::world& world) {

	}

	void world_renderer::deinit() {
		m_texture_atlas.clear();
		m_layers.clear();
	}

	void world_renderer::update() {
		detail::render_ctx render_ctx(m_renderer);
		m_renderer.set_target(m_target);
		_update();
	}

	void world_renderer::_update() {

	}
}
