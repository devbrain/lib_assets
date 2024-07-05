//
// Created by igor on 7/4/24.
//

#ifndef WORLD_RENDERER_TILE_HH
#define WORLD_RENDERER_TILE_HH

#include <cstdint>
#include <tuple>
#include <world_renderer/types.hh>

namespace neutrino::tiled {
	struct tile {
		tile();
		explicit tile(tile_id_t tile_id);
		tile(tiles_texture_id_t tex_id, tile_id_t tile_id);
		tile(tiles_texture_id_t tex_id, tile_id_t tile_id, bool hor_flip,
		     bool vert_flip, bool diag_flip, int animated);

		tiles_texture_id_t m_texture;
		tile_id_t m_id;

		union {
			struct {
				int hor_flip: 1;
				int vert_flip: 1;
				int diag_flip: 1;
				int animated: 1;
			} flags;

			uint8_t bit_flags;
		} info{0};
	};

	static_assert(sizeof(tile) == 8);

	inline
	bool operator == (const tile& a, const tile& b) {
		return (a.m_texture == b.m_texture) && (a.m_id == b.m_id);
	}
	inline
	bool operator < (const tile& a, const tile& b) {
		return std::make_tuple(a.m_texture, a.m_id) < std::make_tuple(b.m_texture, b.m_id);
	}
}
#endif
