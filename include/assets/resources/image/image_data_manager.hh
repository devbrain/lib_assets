//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_

#include "sdlpp/surface.hh"
#include "image_info.hh"
#include "assets/resources/data_manager.hh"
#include "assets/assets_export.h"

namespace assets {
	class ASSETS_EXPORT image_data_manager : public data_manager<neutrino::sdl::surface, image_info> {
	 public:
		image_data_manager();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_
