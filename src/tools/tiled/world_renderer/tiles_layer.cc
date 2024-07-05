//
// Created by igor on 7/4/24.
//

#include "tiles_layer.hh"
#include <bsw/exception.hh>

namespace neutrino::tiled {
	tiles_layer::tiles_layer(tile_coord_t w, tile_coord_t h, world_coords_t tile_width, world_coords_t tile_height)
		: m_width(w),
		  m_height(h),
		  m_tile_width(tile_width),
		  m_tile_height(tile_height),
		  m_tiles(static_cast<std::size_t>(w.value_of()) * static_cast<std::size_t>(h.value_of()), tile(tile_id_t(EMPTY_TILE_VALUE))) {
	}

	tile_coord_t tiles_layer::get_width() const {
		return m_width;
	}

	tile_coord_t tiles_layer::get_height() const {
		return m_height;
	}

	world_coords_t tiles_layer::get_tile_width() const {
		return m_tile_width;
	}

	world_coords_t tiles_layer::get_tile_height() const {
		return m_tile_height;
	}

	tile& tiles_layer::at(tile_coord_t x, tile_coord_t y) {
		ENFORCE(x < m_width && y < m_height);
		return m_tiles[static_cast<std::size_t>(y.value_of())*static_cast<std::size_t>(m_width.value_of()) + static_cast<std::size_t>(x.value_of())];
	}

	bool tiles_layer::is_empty(const tile& t) {
		return t.m_id.value_of() == EMPTY_TILE_VALUE;
	}

	void tiles_layer::clear(tile_coord_t x, tile_coord_t y) {
		at(x, y).m_id = tile_id_t(EMPTY_TILE_VALUE);
	}
}
