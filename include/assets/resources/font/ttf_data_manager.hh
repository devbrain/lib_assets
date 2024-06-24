//
// Created by igor on 6/24/24.
//

#ifndef TTF_FONT_DATA_MANAGER_HH
#define TTF_FONT_DATA_MANAGER_HH

#include <assets/resources/data_manager.hh>
#include <sdlpp/video/ttf.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT ttf_data_manager : public data_manager <sdl::ttf> {
		public:
			ttf_data_manager();
	};
}

#endif //TTF_FONT_DATA_MANAGER_HH
