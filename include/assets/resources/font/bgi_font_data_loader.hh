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

	enum class bgi_font_t {
		TRIPLEX_FONT, // Stroked triplex font
		SMALL_FONT, // 	Stroked small font
		SANS_SERIF_FONT, //	Stroked sans-serif font
		GOTHIC_FONT, // Stroked gothic font
		SCRIPT_FONT, // Stroked script font
		SIMPLEX_FONT, // Stroked triplex script font
		TRIPLEX_SCR_FONT, // Stroked triplex script font
		COMPLEX_FONT, // Stroked complex font
		EUROPEAN_FONT, // Stroked European font
		BOLD_FONT // Stroked bold font
	};

	ASSETS_EXPORT const bgi_font& load_standard_bgi_font(bgi_font_t name);
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_BGI_FONT_DATA_MANAGER_HH
