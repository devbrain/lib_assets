//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_RESOURCE_LOADER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_RESOURCE_LOADER_HH_

#include <sdlpp/surface.hh>
#include <assets/resources/abstract_resource_loader.hh>
#include <assets/assets_export.h>

namespace assets {
	struct ASSETS_EXPORT image_info {
		unsigned int width;
		unsigned int height;
		neutrino::sdl::pixel_format pixel_format;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_RESOURCE_LOADER_HH_
