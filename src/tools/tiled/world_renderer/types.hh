//
// Created by igor on 7/4/24.
//

#ifndef WORL_RENDERER_TYPES_HH
#define WORL_RENDERER_TYPES_HH

#include <cstdint>
#include <assets/resources/world/types.hh>

namespace neutrino::tiled {
	d_DECLARE_TILED_TYPE_ORDERED(int, world_coords_t);
	d_DECLARE_TILED_TYPE_ORDERED(uint8_t, tiles_texture_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(uint8_t, image_texture_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(uint16_t, tile_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(int, tile_coord_t);
}

#endif
