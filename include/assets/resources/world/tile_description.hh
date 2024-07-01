//
// Created by igor on 06/07/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_DESCRIPTION_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_DESCRIPTION_HH

#include <cstdint>
#include <vector>
#include <optional>
#include <bitflags/bitflags.hpp>
#include <assets/resources/world/types.hh>

namespace neutrino::assets {
  BEGIN_RAW_BITFLAGS(flip_t)
    RAW_FLAG(HORIZONTAL)
    RAW_FLAG(VERTICAL)
    RAW_FLAG(DIAGONAL)
  END_RAW_BITFLAGS(flip_t)


  struct tile_description {
    tile_description(texture_id_t atexture_id, tile_id_t atile_id, flip_t aflip)
    : texture_id(atexture_id), tile_id(atile_id), flip(aflip) {}

    texture_id_t    texture_id;
    tile_id_t       tile_id;
    flip_t          flip;
  };

  using tile_t = std::optional<tile_description>;
}

#endif
