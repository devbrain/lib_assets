//
// Created by igor on 6/24/24.
//

#ifndef TTF_FONT_DATA_MANAGER_HH
#define TTF_FONT_DATA_MANAGER_HH

#include <assets/resources/data_loader.hh>
#include <sdlpp/video/ttf.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT ttf_data_loader : public data_loader <sdl::ttf> {
		public:
			ttf_data_loader();
	};
}

#endif //TTF_FONT_DATA_MANAGER_HH
