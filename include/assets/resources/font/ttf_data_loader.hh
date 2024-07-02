//
// Created by igor on 6/24/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_TTF_FONT_DATA_MANAGER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_TTF_FONT_DATA_MANAGER_HH

#include <assets/resources/data_loader.hh>
#include <sdlpp/video/ttf.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	struct ASSETS_EXPORT point_size {
		explicit point_size(int ptsize_)
			: ptsize(ptsize_),
			  hdpi(0),
			  vdpi(0) {
		}

		point_size(int ptsize_, unsigned int hdpi_)
			: ptsize(ptsize_),
			  hdpi(hdpi_),
			  vdpi(0) {
		}

		point_size(int ptsize_, unsigned int hdpi_, unsigned int vdpi_)
			: ptsize(ptsize_),
			  hdpi(hdpi_),
			  vdpi(vdpi_) {
		}

		int ptsize;
		unsigned int hdpi;
		unsigned int vdpi;
	};

	class ASSETS_EXPORT ttf_data_loader : public data_loader <sdl::ttf, point_size> {
		public:
			ttf_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_TTF_FONT_DATA_MANAGER_HH
