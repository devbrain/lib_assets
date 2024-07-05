//
// Created by igor on 7/5/24.
//

#ifndef  WORLD_LOADER_HH
#define  WORLD_LOADER_HH

#include <vector>
#include <tuple>
#include <assets/resources/world/world.hh>
#include <sdlpp/video/surface.hh>
#include <sdlpp/video/geometry.hh>
#include <world_renderer/types.hh>
#include <world_renderer/world_model.hh>

namespace neutrino::tiled {
	struct atlas_builder {
		struct texture_material {
			texture_material(tiles_texture_id_t tex_id, const std::vector<sdl::rect>& coords,
				sdl::surface&& image)
				: m_tex_id(tex_id),
				  m_coords(coords),
				  m_image(std::move(image)) {
			}
			texture_material(tiles_texture_id_t tex_id, sdl::surface&& image)
				: m_tex_id(tex_id),
				  m_image(std::move(image)) {
			}
			tiles_texture_id_t m_tex_id;
			std::vector<sdl::rect> m_coords;
			sdl::surface m_image;
		};

		std::vector<texture_material> m_material;
	};


	std::tuple<atlas_builder, world_model> world_loader(assets::world&& w);

}



#endif
