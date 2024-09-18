//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_ROM_FONT_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_ROM_FONT_HH

#include <array>
#include <string>
#include <assets/resources/detail/bitmap2d.hh>
#include <assets/assets_export.h>
#include <assets/resources/palette/palette_data_loader.hh>
#include <sdlpp/video/geometry.hh>
#include <sdlpp/video/surface.hh>
#include <sdlpp/video/color.hh>

namespace neutrino::assets {
	class ASSETS_EXPORT rom_font_glyph : public detail::bitmap2d {
		public:
			rom_font_glyph() = default;
			rom_font_glyph(unsigned w, unsigned h);
	};

	class ASSETS_EXPORT rom_font_8x8_glyph : public rom_font_glyph {
		public:
			rom_font_8x8_glyph();
	};

	class ASSETS_EXPORT rom_font_8x14_glyph : public rom_font_glyph {
		public:
			rom_font_8x14_glyph();
	};

	class ASSETS_EXPORT rom_font_8x16_glyph : public rom_font_glyph {
		public:
			rom_font_8x16_glyph();
	};

	namespace detail {
		template<typename Glyph>
		class generic_romfont {
			public:
				[[nodiscard]] unsigned int get_width() const {
					return m_glyphs[0].get_width();
				}

				[[nodiscard]] unsigned int get_height() const {
					return m_glyphs[0].get_height();
				}

				void set_glyph(const Glyph& g, unsigned idx) {
					m_glyphs[idx] = g;
				}

				[[nodiscard]] const Glyph& get_glyph(unsigned ch) const {
					return m_glyphs[ch];
				}

			private:
				std::array <Glyph, 256> m_glyphs;
		};

		template<>
		class generic_romfont <rom_font_glyph> {
			public:
				generic_romfont([[maybe_unused]] unsigned w, [[maybe_unused]] unsigned h)
					: m_width(), m_height() {
				}

				[[nodiscard]] unsigned int get_width() const {
					return m_width;
				}

				[[nodiscard]] unsigned int get_height() const {
					return m_height;
				}

				void set_glyph(const rom_font_glyph& g, unsigned idx) {
					m_glyphs[idx] = g;
				}

				[[nodiscard]] const rom_font_glyph& get_glyph(unsigned ch) const {
					return m_glyphs[ch];
				}

			private:
				unsigned int m_width;
				unsigned int m_height;
				std::array <rom_font_glyph, 256> m_glyphs{};
		};
	}

	class ASSETS_EXPORT rom_font : public detail::generic_romfont <rom_font_glyph> {
		public:
		rom_font(unsigned w, unsigned h)
			: generic_romfont(w, h) {}
	};

	class ASSETS_EXPORT rom_font_8x8 : public detail::generic_romfont <rom_font_8x8_glyph> {
	};

	class ASSETS_EXPORT rom_font_8x14 : public detail::generic_romfont <rom_font_8x14_glyph> {
	};

	class ASSETS_EXPORT rom_font_8x16 : public detail::generic_romfont <rom_font_8x16_glyph> {
	};

	template <typename Glyph>
	sdl::area_type get_rendered_dimensions(const detail::generic_romfont<Glyph>& glyphs, const std::string& str) {
		const unsigned int w = glyphs.get_width();
		const unsigned int len = str.size();
		unsigned int h = glyphs.get_height();
		return {static_cast <int>(w * len), static_cast <int>(h)};
	}

	template <typename Glyph>
	sdl::surface render_string_with_romfont(const detail::generic_romfont<Glyph>& glyphs,
		const sdl::pixel_format& px, const sdl::color& fg_color, const sdl::color& bg_color,
		const std::string& str) {
		auto dims = get_rendered_dimensions(glyphs, str);

		sdl::surface srf(dims.w, dims.h, px);
		srf.set_palette(load_standard_vga_palette());

		const auto fg = srf.map_color(fg_color);
		const auto bg = srf.map_color(bg_color);
		for (std::size_t i=0; i<str.size(); i++) {
			const int ch = static_cast<uint8_t>(str[i]);
			const auto& glyph = glyphs.get_glyph(ch);
			const auto w = glyph.get_width();
			const auto h = glyph.get_height();
			const auto offs = i*w;
			for (unsigned y = 0; y<h; y++) {
				uint8_t* row = static_cast <uint8_t*>(srf->pixels) + y * srf->pitch;
				for (unsigned x = 0; x<w; x++) {
					auto pixel = glyph.is_set(x, y) ? fg : bg;
					if (srf->format->BytesPerPixel == 1) {
						auto* target_pixel = reinterpret_cast <uint8_t*>(row + (offs + x) * srf->format->BytesPerPixel);
						*target_pixel = pixel;
					} else if (srf->format->BytesPerPixel == 2) {
						auto* target_pixel = reinterpret_cast <uint16_t*>(row + (offs + x) * srf->format->BytesPerPixel);
						*target_pixel = pixel;
					} else {
						auto* target_pixel = reinterpret_cast <uint32_t*>(row + (offs + x) * srf->format->BytesPerPixel);
						*target_pixel = pixel;
					}
				}
			}

		}
		return srf;
	}

	ASSETS_EXPORT sdl::surface render_romfont_8x8(const std::string& str, const sdl::color& fg_color, const sdl::color& bg_color);
	ASSETS_EXPORT sdl::surface render_romfont_8x14(const std::string& str, const sdl::color& fg_color, const sdl::color& bg_color);
	ASSETS_EXPORT sdl::surface render_romfont_8x16(const std::string& str, const sdl::color& fg_color, const sdl::color& bg_color);
}

#endif
