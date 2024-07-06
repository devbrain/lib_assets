//
// Created by igor on 7/4/24.
//

#ifndef WORL_RENDERER_TYPES_HH
#define WORL_RENDERER_TYPES_HH

#include <cstdint>
#include <limits>
#include <assets/resources/world/types.hh>

namespace neutrino::tiled {
	namespace detail {
		using underlying_tile_type = uint16_t;
	}
	d_DECLARE_TILED_TYPE_ORDERED(int, world_coords_t);
	d_DECLARE_TILED_TYPE_ORDERED(uint8_t, texture_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(detail::underlying_tile_type, tile_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(int, tile_coord_t);
	inline constexpr auto EMPTY_TILE_VALUE = std::numeric_limits<detail::underlying_tile_type>::max();
}

#endif
