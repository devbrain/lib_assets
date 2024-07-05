//
// Created by igor on 7/4/24.
//

#ifndef  WORLD_RENDERER_TEXTURE_ATLAS_HH
#define  WORLD_RENDERER_TEXTURE_ATLAS_HH

#include <vector>
#include <tuple>

#include <sdlpp/video/texture.hh>
#include <world_renderer/tile.hh>

namespace neutrino::tiled {
	class texture_atlas {
		public:
			texture_atlas() = default;
			void clear();

			void add(sdl::texture&& texture, std::vector<sdl::rect>&& rects);
			[[nodiscard]] std::tuple<const sdl::texture*, sdl::rect> get(const tile& tl) const;
		private:
			struct tiles_map {
				tiles_map(std::vector<sdl::rect>&& rects, sdl::texture&& texture)
					: m_rects(std::move(rects)),
					  m_texture(std::move(texture)) {
				}

				std::vector<sdl::rect> m_rects;
				sdl::texture m_texture;
			};
		std::vector<tiles_map> m_textures;
	};
}



#endif
