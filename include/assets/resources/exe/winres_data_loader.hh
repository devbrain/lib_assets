//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WINRES_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WINRES_DATA_MANAGER_HH_

#include <assets/resources/data_loader.hh>
#include <assets/resources/exe/windows_resources.hh>
#include <assets/assets_export.h>


namespace neutrino::assets {
 class ASSETS_EXPORT winres_data_loader : public data_loader<windows_resource_directory> {
	 public:
		winres_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_DATA_MANAGER_HH_
