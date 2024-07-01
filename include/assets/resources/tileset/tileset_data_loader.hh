//
// Created by igor on 6/23/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_DATA_LOADER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_DATA_LOADER_HH

#include <assets/resources/tileset/tileset.hh>
#include <assets/resources/data_loader.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT tileset_data_loader : public data_loader <tileset> {
		public:
			tileset_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_DATA_LOADER_HH
