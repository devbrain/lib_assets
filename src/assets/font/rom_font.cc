//
// Created by igor on 7/3/24.
//
#include <assets/resources/font/rom_font.hh>

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
}
