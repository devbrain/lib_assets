//
// Created by igor on 6/23/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_BGI_FONT_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_BGI_FONT_HH

#include <vector>
#include <map>
#include <sdlpp/video/geometry.hh>

namespace neutrino::assets {
	class bgi_font {
		public:
			struct command {
				bool move;
				sdl::point coords;
			};
			struct glyph_data {
				std::vector <command> stroke_commands;
				sdl::area_type dimension;
			};
			std::map <char, glyph_data> glyphs;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_BGI_FONT_HH
