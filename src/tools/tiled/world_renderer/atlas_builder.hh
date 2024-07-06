//
// Created by igor on 7/5/24.
//

#ifndef ATLAS_BUILDER_HH
#define ATLAS_BUILDER_HH

#include <vector>
#include <sdlpp/video/surface.hh>
#include <sdlpp/video/geometry.hh>
#include <world_renderer/types.hh>


namespace neutrino::tiled {
	struct atlas_builder {
		struct texture_material {
			texture_material(texture_id_t tex_id, const std::vector<sdl::rect>& coords,
				sdl::surface&& image)
				: m_tex_id(tex_id),
				  m_coords(coords),
				  m_image(std::move(image)) {
			}
			texture_material(texture_id_t tex_id, sdl::surface&& image)
				: m_tex_id(tex_id),
				  m_image(std::move(image)) {
			}
			texture_id_t m_tex_id;
			std::vector<sdl::rect> m_coords;
			sdl::surface m_image;
		};

		std::vector<texture_material> m_material;
	};
}
#endif
