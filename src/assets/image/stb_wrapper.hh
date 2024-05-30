//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_SRC_ASSETS_IMAGE_STB_WRAPPER_HH_
#define ASSETS_SRC_ASSETS_IMAGE_STB_WRAPPER_HH_

#include <iosfwd>
#include "assets/resources/image/image_info.hh"

namespace assets::detail {

	bool is_gif(std::istream& is);
	image_info gif_info(std::istream& is);
	neutrino::sdl::surface gif_load(std::istream& is);

	bool is_tga(std::istream& is);
	image_info tga_info(std::istream& is);
	neutrino::sdl::surface tga_load(std::istream& is);

	bool is_png(std::istream& is);
	image_info png_info(std::istream& is);
	neutrino::sdl::surface png_load(std::istream& is);

	bool is_jpeg(std::istream& is);
	image_info jpeg_info(std::istream& is);
	neutrino::sdl::surface jpeg_load(std::istream& is);

	bool is_psd(std::istream& is);
	image_info psd_info(std::istream& is);
	neutrino::sdl::surface psd_load(std::istream& is);

}

#endif //ASSETS_SRC_ASSETS_IMAGE_STB_WRAPPER_HH_
