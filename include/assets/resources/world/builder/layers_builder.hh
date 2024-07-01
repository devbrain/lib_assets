//
// Created by igor on 20/09/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_LAYERS_BUILDER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_LAYERS_BUILDER_HH

#include <assets/resources/world/world_layer.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
  class ASSETS_EXPORT layers_builder {
    public:
      layers_builder(unsigned int width, unsigned int height);
      layers_builder& add_tile(unsigned int x, unsigned int y, texture_id_t texture_id, tile_id_t tile_id, flip_t flip);

      [[nodiscard]] tiles_layer build() const;
    private:
      unsigned int m_width;
      unsigned int m_height;
      // x,y,tile_description
      using tile_info = std::tuple<unsigned int, unsigned int, tile_description>;
      std::vector<tile_info> m_tiles;
  };
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_LAYERS_BUILDER_HH
