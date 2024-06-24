//
// Created by igor on 6/24/24.
//

#ifndef BGI_FONT_DATA_MANAGER_HH
#define BGI_FONT_DATA_MANAGER_HH

#include <assets/resources/data_manager.hh>
#include <assets/resources/font/bgi_font.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT bgi_font_data_manager : public data_manager <bgi_font> {
		public:
			bgi_font_data_manager();
	};
}

#endif //BGI_FONT_DATA_MANAGER_HH
