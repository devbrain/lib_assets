//
// Created by igor on 7/4/24.
//

#include "tile.hh"

namespace neutrino::tiled {
	tile::tile()
		: m_texture(0),
		  m_id(0) {
	}

	tile::tile(tile_id_t tile_id)
		: m_texture(0),
		  m_id(tile_id) {
	}

	tile::tile(tiles_texture_id_t tex_id, tile_id_t tile_id)
		: m_texture(tex_id),
		  m_id(tile_id) {
	}

	tile::tile(tiles_texture_id_t tex_id, tile_id_t tile_id, bool hor_flip, bool vert_flip, bool diag_flip,
	           int animated)
		: m_texture(tex_id),
		  m_id(tile_id) {
		info.flags.animated = animated ? 1 : 0;
		info.flags.diag_flip = diag_flip ? 1 : 0;
		info.flags.hor_flip = hor_flip ? 1 : 0;
		info.flags.vert_flip = vert_flip ? 1 : 0;
	}
}
