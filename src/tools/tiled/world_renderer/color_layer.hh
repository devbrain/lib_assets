//
// Created by igor on 7/4/24.
//

#ifndef  COLOR_LAYER_HH
#define  COLOR_LAYER_HH

#include <sdlpp/video/color.hh>

namespace neutrino::tiled {
	class color_layer {
		public:
			explicit color_layer(const sdl::color& c);
			[[nodiscard]] sdl::color get_color() const;
		private:
			sdl::color m_color;
	};
}

#endif
