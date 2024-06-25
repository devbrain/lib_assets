//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_DATA_MANAGER_HH_

#include <vector>
#include <assets/resources/data_loader.hh>
#include <assets/resources/exe/unpacked_exe.hh>
#include <assets/assets_export.h>


namespace neutrino::assets {
 class ASSETS_EXPORT exe_data_loader : public data_loader<unpacked_exe> {
	 public:
		exe_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_DATA_MANAGER_HH_
