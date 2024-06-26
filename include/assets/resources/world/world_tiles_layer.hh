//
// Created by igor on 20/09/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_WORLD_LAYER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_WORLD_LAYER_HH

#include <vector>
#include <assets/resources/world/tile_description.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
  class layers_builder;

  class ASSETS_EXPORT tiles_layer {
    friend class layers_builder;
    public:
      tiles_layer(const tiles_layer&) = delete;
      tiles_layer& operator = (const tiles_layer&) = delete;

      tiles_layer(tiles_layer&&) = default;
      tiles_layer& operator = (tiles_layer&&) = default;

      /**
       * Returns tile at coords <x,y>. Throws exception if the coords are out of bounds
       * @param x - x coordinate of the tile
       * @param y - y coordinate of the tile
       * @return
       */
      [[nodiscard]] const tile_t& at(unsigned int x, unsigned int y) const;

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
    private:
      /**
       * Constructor
       * @param width width in tiles
       * @param height height in tiles
       */
      tiles_layer(unsigned int width, unsigned int height);

      [[nodiscard]] tile_t& at(unsigned int x, unsigned int y);
    private:
      [[nodiscard]] std::size_t get_index(unsigned int x, unsigned int y) const;
    private:
      unsigned int m_width;
      unsigned int m_height;
      std::vector<tile_t> m_tiles;
  };
} // ns neutrino::tiled

#endif //INCLUDE_NEUTRINO_TILED_WORLD_LAYER_HH
