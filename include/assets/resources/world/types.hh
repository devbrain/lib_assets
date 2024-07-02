//
// Created by igor on 17/07/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_TYPES_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_TYPES_HH

#include <cstddef>
#include <strong_type/strong_type.hpp>
#include <bsw/macros.hh>

#define d_DECLARE_TILED_TYPE(TYPE, NAME)                    \
    namespace detail    {                                   \
        struct PPCAT(NAME,_){};                             \
    }                                                       \
    using NAME = strong::type<TYPE, detail::PPCAT(NAME,_), strong::ostreamable>

#define d_DECLARE_TILED_TYPE_ORDERED(TYPE, NAME)            \
    namespace detail    {                                   \
        struct PPCAT(NAME,_){};                             \
    }                                                       \
    using NAME = strong::type<TYPE, detail::PPCAT(NAME,_), strong::ordered, strong::ostreamable, strong::bicrementable, strong::equality>

namespace neutrino::assets {
	d_DECLARE_TILED_TYPE_ORDERED(std::size_t, texture_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(std::size_t, layer_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(std::size_t, tile_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(std::size_t, image_id_t);
    d_DECLARE_TILED_TYPE_ORDERED(int, object_id_t);
}

#endif
