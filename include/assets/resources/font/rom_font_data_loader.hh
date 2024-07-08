//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_ROM_FONT_DATA_LOADER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_ROM_FONT_DATA_LOADER_HH

#include <assets/resources/data_loader.hh>
#include <assets/resources/font/rom_font.hh>
#include <assets/resources/resource_from_stream.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	struct ASSETS_EXPORT rom_font_resource : resource_from_stream {
		rom_font_resource(std::istream& is, unsigned w, unsigned h)
			: width(w), height(h), m_stream(is) {}

		static rom_font_resource make_8x8(std::istream& is);
		static rom_font_resource make_8x14(std::istream& is);
		static rom_font_resource make_8x16(std::istream& is);

		[[nodiscard]] std::istream& get_stream() const override;
		unsigned width;
		unsigned height;
		std::istream& m_stream;
	};

	ASSETS_ENABLE_ISTREAM_STATE(rom_font_resource);

	class ASSETS_EXPORT rom_font_data_loader : public data_loader <rom_font, rom_font_resource> {
		public:
			rom_font_data_loader();
	};

	class ASSETS_EXPORT rom_font_8x8_data_loader : public data_loader <rom_font_8x8> {
		public:
			rom_font_8x8_data_loader();
	};

	class ASSETS_EXPORT rom_font_8x14_data_loader : public data_loader <rom_font_8x14> {
		public:
			rom_font_8x14_data_loader();
	};

	class ASSETS_EXPORT rom_font_8x16_data_loader : public data_loader <rom_font_8x16> {
		public:
			rom_font_8x16_data_loader();
	};

	ASSETS_EXPORT const rom_font_8x8& get_standard_rom_font_8x8();
	ASSETS_EXPORT const rom_font_8x14& get_standard_rom_font_8x14();
	ASSETS_EXPORT const rom_font_8x16& get_standard_rom_font_8x16();
}

#endif
