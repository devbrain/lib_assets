//
// Created by igor on 6/2/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_INIT_HH_
#define ASSETS_INCLUDE_ASSETS_INIT_HH_

#include <assets/assets_export.h>

namespace assets {
	ASSETS_EXPORT void assets_init();
	ASSETS_EXPORT void assets_done();

	struct ASSETS_EXPORT initializer {
		initializer();
		~initializer();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_INIT_HH_
