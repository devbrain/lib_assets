//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_DATA_MANAGER_HH_

#include <vector>
#include <string>
#include <assets/resources/data_manager.hh>
#include <assets/resources/exe/exe_info.hh>
#include <assets/assets_export.h>


namespace assets {
 class ASSETS_EXPORT exe_data_manager : public data_manager<std::vector<char>, exe_info> {
	 public:
		exe_data_manager();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_DATA_MANAGER_HH_