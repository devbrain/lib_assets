//
// Created by igor on 7/7/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_PIXELS_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_PIXELS_HH

#include <vector>
#include <map>
#include <tuple>
#include <cstdint>
#include <sdlpp/video/geometry.hh>
#include <sdlpp/video/color.hh>
#include <sdlpp/video/surface.hh>

#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT pixels {
		public:
			pixels(std::size_t w, std::size_t h);

			pixels(pixels&&) = default;
			pixels& operator =(pixels&&) = default;

			[[nodiscard]] std::size_t get_width() const;
			[[nodiscard]] std::size_t get_height() const;
			[[nodiscard]] const uint8_t* data() const;
			[[nodiscard]] uint8_t* data();
			[[nodiscard]] std::size_t get_size() const;
		private:
			std::size_t m_width;
			std::size_t m_height;
			std::vector <uint8_t> m_pixels;
	};

	ASSETS_EXPORT std::tuple <sdl::surface, std::vector <sdl::rect>> render_pixels_to_surface(
		const std::vector <pixels>& data,
		const sdl::color& fg_color, const sdl::color& bg_color);
}

#endif
