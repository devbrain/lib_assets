//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_SRC_ASSETS_IMAGE_STB_WRAPPER_HH_
#define ASSETS_SRC_ASSETS_IMAGE_STB_WRAPPER_HH_

#include <iosfwd>
#include <sdlpp/video/surface.hh>

namespace neutrino::assets::detail {
	bool is_gif(std::istream& is);
	sdl::surface gif_load(std::istream& is);

	bool is_tga(std::istream& is);
	sdl::surface tga_load(std::istream& is);

	bool is_png(std::istream& is);
	sdl::surface png_load(std::istream& is);

	bool is_jpeg(std::istream& is);
	sdl::surface jpeg_load(std::istream& is);

	bool is_psd(std::istream& is);
	sdl::surface psd_load(std::istream& is);
}

#endif //ASSETS_SRC_ASSETS_IMAGE_STB_WRAPPER_HH_
