//
// Created by igor on 06/07/2021.
//

#ifndef NEUTRINO_TILED_WORLD_WORLD_HH
#define NEUTRINO_TILED_WORLD_WORLD_HH

#include <assets/resources/world/world_layer.hh>
#include <assets/resources/world/world_props.hh>
#include <sdlpp/video/color.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
  class world_builder;

  class ASSETS_EXPORT world {
    friend class world_builder;
    public:
      world(const world&) = delete;
      world& operator = (const world&) = delete;

      world(world&&) = default;

      template <class Functor>
      void visit_layers(Functor&& visitor);

      [[nodiscard]] orientation_t orientation () const;
      [[nodiscard]] unsigned int width () const;
      [[nodiscard]] unsigned int height () const;
      [[nodiscard]] unsigned int tile_width () const;
      [[nodiscard]] unsigned int tile_height () const;
      [[nodiscard]] const sdl::color& bg_color () const;
      [[nodiscard]] render_order_t render_order () const;
      [[nodiscard]] unsigned int hex_side_length () const;
      [[nodiscard]] stagger_axis_t stagger_axis () const;
      [[nodiscard]] stagger_index_t stagger_index () const;
      [[nodiscard]]  bool infinite () const;
    private:
    public:
      world (orientation_t orientation, unsigned int width, unsigned int height,
             unsigned int tilewidth, unsigned int tileheight, const sdl::color& bgcolor,
             render_order_t render_order, unsigned int hex_side_length, stagger_axis_t axis,
             stagger_index_t index, bool infinite);
    private:
      std::vector<tiles_layer> m_layers;
      const orientation_t m_orientation;

      const unsigned m_width;
      const unsigned m_height;

      const unsigned m_tilewidth;
      const unsigned m_tileheight;

      const sdl::color m_bgcolor;

      const render_order_t m_render_order;

      const unsigned m_hex_side_length;
      const stagger_axis_t m_axis;
      const stagger_index_t m_index;

      const bool m_infinite;
  };
  // ==========================================================================
  template <class Functor>
  void world::visit_layers(Functor&& visitor) {
    for (const auto& the_layer : m_layers) {
      visitor(the_layer);
    }
  }


}

#endif
