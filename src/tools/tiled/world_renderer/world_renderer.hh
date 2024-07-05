//
// Created by igor on 7/4/24.
//

#ifndef  WORLD_RENDERER_HH
#define  WORLD_RENDERER_HH

#include <variant>
#include <vector>

#include <sdlpp/video/render.hh>
#include <sdlpp/video/geometry.hh>
#include <assets/resources/world/world.hh>
#include <world_renderer/texture_atlas.hh>
#include <world_renderer/color_layer.hh>
#include <world_renderer/image_layer.hh>
#include <world_renderer/tiles_layer.hh>

namespace neutrino::tiled {
	class world_renderer {
		public:
			world_renderer(sdl::renderer& renderer,
			               sdl::area_type& dimensions,
			               sdl::pixel_format pixel_format = sdl::pixel_format(sdl::pixel_format::RGBA8888));

			void init(const assets::world& world);
			void deinit();

			void update();
		private:
			void _update();
			sdl::renderer& m_renderer;
			sdl::area_type m_dimension;
			sdl::texture   m_target;
			texture_atlas  m_texture_atlas;

			using layer_t = std::variant<color_layer, image_layer, tiles_layer>;
			std::vector<layer_t> m_layers;
	};
}

#endif
