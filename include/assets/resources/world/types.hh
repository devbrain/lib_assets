//
// Created by igor on 17/07/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_TYPES_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILE_TYPES_HH

#include <cstddef>
#include <strong_type/strong_type.hpp>
#include <bsw/macros.hh>

namespace neutrino::assets {
    /**
     * @brief the orientation of the map.
     */
    enum class orientation_t {
        UNKNOWN,    /**< Unknown orientation */
        ORTHOGONAL, /**< Orthogonal orientation */
        ISOMETRIC,  /**< Isometric orientation */
        STAGGERED,  /**< Staggered orientation */
        HEXAGONAL,  /**< Hexagonal orientation */
      };

    /**
     * @brief Stagger index of the hexagonal map.
     */
    enum class stagger_index_t {
        ODD,
        EVEN,
      };

    /**
     * @brief Stagger axis of the hexagonal map.
     */
    enum class stagger_axis_t {
        X,
        Y,
      };

    /**
     * @brief the render order of the tiles.
     */
    enum class render_order_t {
        RIGHT_DOWN, /**< Right down order */
        RIGHT_UP,   /**< Right up order */
        LEFT_DOWN,  /**< Left down order */
        LEFT_UP,    /**< Left up order */
      };
}

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
    d_DECLARE_TILED_TYPE_ORDERED(std::size_t, local_tile_id_t);
	d_DECLARE_TILED_TYPE_ORDERED(std::size_t, image_id_t);
    d_DECLARE_TILED_TYPE_ORDERED(int, object_id_t);
}

#endif
