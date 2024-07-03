//
// Created by igor on 20/09/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_WORLD_LAYER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_WORLD_LAYER_HH

#include <vector>
#include <assets/resources/world/tile_description.hh>
#include <assets/resources/world/world_component.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT tiles_layer : public component {
		public:
			tiles_layer(unsigned int width, unsigned int height, tile_id_t empty_id);

			tiles_layer(const tiles_layer&) = default;
			tiles_layer& operator =(const tiles_layer&) = default;

			tiles_layer(tiles_layer&&) = default;
			tiles_layer& operator =(tiles_layer&&) = default;

			/**
			 * Returns tile at coords <x,y>. Throws exception if the coords are out of bounds
			 * @param x - x coordinate of the tile
			 * @param y - y coordinate of the tile
			 * @return
			 */
			[[nodiscard]] const tile_description& at(unsigned int x, unsigned int y) const;

			/**
			 * width in tiles
			 * @return width in tiles
			 */
			[[nodiscard]] unsigned int width() const noexcept;

			/**
			 * height in tiles
			 * @return height in tiles
			 */
			[[nodiscard]] unsigned int height() const noexcept;


			[[nodiscard]] tile_description& at(unsigned int x, unsigned int y);

		private:
			[[nodiscard]] std::size_t get_index(unsigned int x, unsigned int y) const;

		private:
			unsigned int m_width;
			unsigned int m_height;
			std::vector <tile_description> m_tiles;
	};
} // ns neutrino::tiled

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_WORLD_LAYER_HH
