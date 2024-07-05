//
// Created by igor on 7/4/24.
//

#ifndef  WORLD_RENDERER_TEXTURE_ATLAS_HH
#define  WORLD_RENDERER_TEXTURE_ATLAS_HH

#include <vector>
#include <map>

#include <sdlpp/video/texture.hh>
#include <sdlpp/video/render.hh>
#include <sdlpp/video/surface.hh>
#include <world_renderer/types.hh>

namespace neutrino::tiled {
	class texture_atlas {
		public:
			texture_atlas() = default;

			void clear();
		private:
			struct tiles_map {
				std::map<tile_id_t, sdl::rect> m_rects;
				sdl::texture m_texture;
			};
		std::vector<tiles_map> m_tiled_textures;
		std::vector<sdl::texture> m_image_textures;
	};
}



#endif
