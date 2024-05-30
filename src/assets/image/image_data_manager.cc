//
// Created by igor on 5/29/24.
//

#include "assets/resources/image/image_data_manager.hh"
#include "standard_image_resource_loader.hh"
#include "pcx.hh"

namespace assets {
	image_data_manager::image_data_manager () {
		register_loader ("GIF", std::make_unique<gif_image_resource_loader>());
		register_loader ("TGA", std::make_unique<tga_image_resource_loader>());
		register_loader ("PNG", std::make_unique<png_image_resource_loader>());
		register_loader ("JPG", std::make_unique<jpeg_image_resource_loader>());
		register_loader ("PSD", std::make_unique<psd_image_resource_loader>());
		register_loader ("PCX", std::make_unique<pcx_image_resource_loader>());
	}
}