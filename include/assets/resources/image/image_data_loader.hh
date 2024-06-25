//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_

#include <sdlpp/video/surface.hh>
#include <assets/resources/data_loader.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT image_data_loader : public data_loader<sdl::surface> {
	 public:
		image_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_
