//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_ROM_FONT_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_ROM_FONT_HH

#include <array>
#include <assets/resources/detail/bitmap2d.hh>
#include <assets/assets_export.h>

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
				[[nodiscard]] unsigned get_width() const {
					return m_glyphs[0].get_width();
				}

				[[nodiscard]] unsigned get_height() const {
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
				generic_romfont(unsigned w, unsigned h)
					: m_width(), m_height() {
				}

				[[nodiscard]] unsigned get_width() const {
					return m_width;
				}

				[[nodiscard]] unsigned get_height() const {
					return m_height;
				}

				void set_glyph(const rom_font_glyph& g, unsigned idx) {
					m_glyphs[idx] = g;
				}

				[[nodiscard]] const rom_font_glyph& get_glyph(unsigned ch) const {
					return m_glyphs[ch];
				}

			private:
				unsigned m_width;
				unsigned m_height;
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
}

#endif
