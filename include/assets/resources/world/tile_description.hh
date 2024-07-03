//
// Created by igor on 06/07/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_DESCRIPTION_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_DESCRIPTION_HH

#include <cstdint>
#include <optional>
#include <bitflags/bitflags.hpp>
#include <assets/resources/world/types.hh>

#include <assets/assets_export.h>

namespace neutrino::assets {
	BEGIN_RAW_BITFLAGS(flip_t)
		RAW_FLAG(HORIZONTAL)
		RAW_FLAG(VERTICAL)
		RAW_FLAG(DIAGONAL)
	END_RAW_BITFLAGS(flip_t)

	struct ASSETS_EXPORT tile_description  {
		tile_description(tile_id_t atile_id, flip_t aflip)
			: tile_id(atile_id), flip(aflip) {
		}

		tile_id_t tile_id;
		flip_t flip;
	};
}

#endif
