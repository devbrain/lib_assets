//
// Created by igor on 6/24/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_BGI_FONT_DATA_MANAGER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_BGI_FONT_DATA_MANAGER_HH

#include <assets/resources/data_loader.hh>
#include <assets/resources/font/bgi_font.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT bgi_font_data_loader : public data_loader <bgi_font> {
		public:
			bgi_font_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_BGI_FONT_DATA_MANAGER_HH
