//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_PALETTE_DATA_LOADER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_PALETTE_DATA_LOADER_HH

#include <sdlpp/video/palette.hh>
#include <assets/assets_export.h>
#include <assets/resources/data_loader.hh>

namespace neutrino::assets {
	class ASSETS_EXPORT palette_data_loader : public data_loader<sdl::palette> {
		public:
			palette_data_loader();
	};

	ASSETS_EXPORT const sdl::palette& load_standard_ega_palette();
	ASSETS_EXPORT const sdl::palette& load_standard_vga_palette();
}

#endif
