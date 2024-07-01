//
// Created by igor on 21/09/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_BUILDER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_BUILDER_HH

#include <assets/resources/world/world.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
  class ASSETS_EXPORT world_builder {
    public:
      world_builder& add(tiles_layer&& alayer);

      world build();

      world_builder& orientation (orientation_t orientation);
      world_builder& width (unsigned int width);
      world_builder& height (unsigned int height);
      world_builder& tile_width (unsigned int tilewidth);
      world_builder& tile_height (unsigned int tileheight);
      world_builder& bg_color (const sdl::color& bgcolor);
      world_builder& render_order (render_order_t m_render_order);
      world_builder& hex_side_length (unsigned int m_hex_side_length);
      world_builder& stagger_axis (stagger_axis_t m_axis);
      world_builder& stagger_index (stagger_index_t m_index);
      world_builder& infinite (bool m_infinite);

    private:
      std::vector<tiles_layer> m_layers;

      orientation_t m_orientation;

      unsigned m_width;
      unsigned m_height;

      unsigned m_tilewidth;
      unsigned m_tileheight;

      sdl::color m_bgcolor;

      render_order_t m_render_order;

      unsigned m_hex_side_length;
      stagger_axis_t m_axis;
      stagger_index_t m_index;

      bool m_infinite;
  };


}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_BUILDER_HH
