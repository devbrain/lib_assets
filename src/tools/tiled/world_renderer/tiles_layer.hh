//
// Created by igor on 7/4/24.
//

#ifndef  TILES_LAYER_HH
#define  TILES_LAYER_HH

#include <vector>
#include <world_renderer/tile.hh>

namespace neutrino::tiled {
	class tiles_layer {
		public:
			tiles_layer(tile_coord_t w,
			            tile_coord_t h,
			            world_coords_t tile_width,
			            world_coords_t tile_height);

			[[nodiscard]] tile_coord_t get_width() const;
			[[nodiscard]] tile_coord_t get_height() const;
			[[nodiscard]] world_coords_t get_tile_width() const;
			[[nodiscard]] world_coords_t get_tile_height() const;

			tile& at(tile_coord_t x, tile_coord_t y);
			[[nodiscard]] static bool is_empty(const tile& t) ;
			void clear(tile_coord_t x, tile_coord_t y);

		private:
			tile_coord_t m_width;
			tile_coord_t m_height;
			world_coords_t m_tile_width;
			world_coords_t m_tile_height;
			std::vector <tile> m_tiles;
	};
}

#endif
