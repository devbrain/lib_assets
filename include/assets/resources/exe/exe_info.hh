//
// Created by igor on 5/30/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_EXE_INFO_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_EXE_INFO_HH_

#include <string>
#include <assets/assets_export.h>

namespace neutrino::assets {
	struct ASSETS_EXPORT exe_info {
		std::string compression_type;
		std::size_t decompressed_size;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_EXE_EXE_INFO_HH_
