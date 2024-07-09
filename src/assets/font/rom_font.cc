//
// Created by igor on 7/3/24.
//
#include <assets/resources/font/rom_font.hh>
#include <assets/resources/font/rom_font_data_loader.hh>

namespace neutrino::assets {
	rom_font_glyph::rom_font_glyph(unsigned w, unsigned h)
		: bitmap2d(w, h) {
	}

	rom_font_8x8_glyph::rom_font_8x8_glyph()
		: rom_font_glyph(8, 8) {
	}

	rom_font_8x14_glyph::rom_font_8x14_glyph()
		: rom_font_glyph(8, 14) {
	}

	rom_font_8x16_glyph::rom_font_8x16_glyph()
		: rom_font_glyph(8, 16) {
	}

	sdl::surface render_romfont_8x8(const std::string& str, const sdl::color& fg_color, const sdl::color& bg_color) {
		return render_string_with_romfont(get_standard_rom_font_8x8(), sdl::pixel_format::make_8bit(), fg_color, bg_color, str);
	}

	sdl::surface render_romfont_8x14(const std::string& str, const sdl::color& fg_color, const sdl::color& bg_color) {
		return render_string_with_romfont(get_standard_rom_font_8x14(), sdl::pixel_format::make_8bit(), fg_color, bg_color, str);
	}

	sdl::surface render_romfont_8x16(const std::string& str, const sdl::color& fg_color, const sdl::color& bg_color) {
		return render_string_with_romfont(get_standard_rom_font_8x16(), sdl::pixel_format::make_8bit(), fg_color, bg_color, str);
	}
}
