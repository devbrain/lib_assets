//
// Created by igor on 27/08/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TEXTURE_ATLAS_BUILDER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TEXTURE_ATLAS_BUILDER_HH

#include <optional>
#include <functional>
#include <vector>
#include <optional>
#include <variant>
#include <string>

#include <sdlpp/video/geometry.hh>
#include <sdlpp/video/color.hh>
#include <sdlpp/video/render.hh>
#include <sdlpp/video/surface.hh>
#include <assets/resources/world/texture_atlas.hh>
#include <assets/assets.hh>

namespace neutrino::assets {
  namespace detail {
    class ASSETS_EXPORT base_image {
      public:
        base_image (std::optional<sdl::color> transparent_color,
                    std::optional<sdl::area_type> dimensions,
                    std::optional<std::string> format);

        [[nodiscard]] std::optional<sdl::color> transparent_color () const noexcept;
        [[nodiscard]] std::optional<sdl::area_type> dimensions () const noexcept;
        [[nodiscard]] std::optional<std::string> format () const noexcept;
      private:
        std::optional<sdl::color> m_transparent_color;
        std::optional<sdl::area_type> m_dimensions;
        std::optional<std::string> m_format;
    };
  }

  class ASSETS_EXPORT named_image : public detail::base_image {
    public:
      named_image (const std::optional<sdl::color>& transparent_color, const std::optional<sdl::area_type>& dimensions,
                   const std::optional<std::string>& format, std::string name);
      [[nodiscard]] const std::string& name () const;
    public:
      std::string m_name;
  };

  class ASSETS_EXPORT image_data : public detail::base_image {
    public:
      image_data (const std::optional<sdl::color>& transparent_color, const std::optional<sdl::area_type>& dimensions,
                  const std::optional<std::string>& format, std::vector<char> data);

      image_data (const image_data&) = delete;
      image_data& operator = (const image_data&) = delete;

      image_data (image_data&&) = default;
      image_data& operator = (image_data&&) = default;

      [[nodiscard]] const std::vector<char>& data () const;
    private:
      std::vector<char> m_data;
  };

  /**
   * Image source for the tilesheet.
   * It may be either raw image data or string
   */
  using image_source_t = std::variant<named_image, image_data>;


  class ASSETS_EXPORT tile_sheet_info {
    public:
      tile_sheet_info (image_source_t image_source,
                       unsigned tile_width,
                       unsigned tile_height,
                       unsigned spacing,
                       unsigned margin,
                       unsigned offset_x,
                       unsigned offset_y,
                       std::size_t num_tiles);

      tile_sheet_info (tile_sheet_info&&) = default;
      tile_sheet_info& operator = (tile_sheet_info&&) = default;

      tile_sheet_info (const tile_sheet_info&) = delete;
      tile_sheet_info& operator = (const tile_sheet_info&) = delete;

      [[nodiscard]] const image_source_t& image_source () const noexcept;

      [[nodiscard]] unsigned tile_width () const noexcept;
      [[nodiscard]] unsigned tile_height () const noexcept;
      [[nodiscard]] unsigned spacing () const noexcept;
      [[nodiscard]] unsigned margin () const noexcept;
      [[nodiscard]] unsigned offset_x () const noexcept;
      [[nodiscard]] unsigned offset_y () const noexcept;
      [[nodiscard]] std::size_t num_tiles () const noexcept;
    private:
      image_source_t m_image_source;
      unsigned m_tile_width;
      unsigned m_tile_height;
      unsigned m_spacing; // The spacing in pixels between the tiles in this tileset
      unsigned m_margin;  // The margin around the tiles in this tileset
      unsigned m_offset_x;
    public:

    private:
      unsigned m_offset_y;
      std::size_t m_num_tiles;
  };

  class ASSETS_EXPORT texture_atlas_builder {
    public:
      texture_atlas_builder() = default;

      texture_atlas_builder(texture_atlas_builder&&) = default;
      texture_atlas_builder& operator = (texture_atlas_builder&&) = default;

      explicit texture_atlas_builder(std::vector<tile_sheet_info> info);

      void add(tile_sheet_info&& inf);

      /**
       * populate texture atlas
       * @param out texture atlas to populate
       * @param renderer SDL renderer
       * @param loader functor to load images by string
       */
      texture_atlas build(sdl::renderer& renderer, const std::function<sdl::surface(const std::string&)>& loader) const;
    private:
      std::vector<tile_sheet_info> m_info;
  };

  texture_atlas_builder& operator << (texture_atlas_builder& builder, tile_sheet_info&& inf);
}

#endif //INCLUDE_NEUTRINO_TILED_WORLD_TILE_SHEET_BUILDER__HH
