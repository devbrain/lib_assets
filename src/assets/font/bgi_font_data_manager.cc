//
// Created by igor on 6/24/24.
//
#include <optional>
#include <limits>
#include <algorithm>
#include <cstdint>
#include <vector>

#include <assets/resources/font/bgi_font_data_manager.hh>
#include <assets/resources/detail/istream_pos_keeper.hh>
#include <bsw/io/binary_reader.hh>
#include <bsw/byte_order.hh>
#include <bsw/exception.hh>

namespace {
	struct header_info {
		uint16_t header_size;
		uint16_t font_size;
	};

	std::optional<header_info> get_bgi_header_info(std::istream& is) {
		neutrino::assets::detail::istream_pos_keeper keeper(is);
		bsw::io::binary_reader rdr(is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
		char h1, h2;
		rdr >> h1 >> h2;
		if (h1 != 'P' || h2 != 'K') {
			return std::nullopt;
		}
		bool found = false;
		for (int i=0; i<253; i++) {
			char ch;
			rdr >> ch;
			if (ch == 0x1A) {
				found = true;
				break;
			}
		}
		if (!found) {
			return std::nullopt;
		}
		header_info hinf {};
		rdr >> hinf.header_size;
		char font_name[4];
		rdr.read_raw(font_name, sizeof(font_name));
		rdr >> hinf.font_size;
		uint8_t font_major;
		rdr >> font_major;
		if (font_major == 0) {
			return std::nullopt;
		}
		return hinf;

	}

}

namespace neutrino::assets {

	class bgi_loader : public abstract_resource_loader<bgi_font> {
		bool accept(std::istream& is) const override {

			return get_bgi_header_info(is).has_value();
		}

		bgi_font load(std::istream& is) const override {
			auto hinf = get_bgi_header_info(is);
			ENFORCE(hinf);
			is.seekg(hinf->header_size, std::ios::beg);
			uint8_t stroke_check;
			bsw::io::binary_reader rdr(is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
			rdr >> stroke_check;
			ENFORCE(stroke_check == 0x2B);
			uint16_t char_count;
			rdr >> char_count;
			char undefined, start_char;
			rdr >> undefined >> start_char;
			uint16_t strokes_offset;
			rdr >> strokes_offset;
			int8_t scan_flag, origin_to_ascender, origin_to_baseline, origin_to_descender;
			rdr >> scan_flag >> origin_to_ascender >> origin_to_baseline >> origin_to_descender;
			char font_name[4];
			rdr.read_raw(font_name, sizeof(font_name));
			rdr >> undefined;

			std::vector<uint16_t> offsets(char_count);

			rdr.read_raw(reinterpret_cast <char*>(offsets.data()), offsets.size()*sizeof(uint16_t));
			bgi_font out;
			for (auto i=0u; i<char_count; i++) {
				auto offset = bsw::byte_order::from_little_endian(offsets[i]);
				is.seekg(static_cast <std::streampos>(hinf->header_size) +
					static_cast <std::streampos>(strokes_offset) +
					static_cast <std::streampos>(offset), std::ios::beg);
				uint8_t b[2];
				is.read(reinterpret_cast <char*>(b), 2);
				bgi_font::glyph_data g;
				char m;
				const int x = 0;
				const int y = origin_to_ascender - origin_to_descender;
				int xc = x;
				int yc = y;
				sdl::point pen(0, 0);
				int min_x = std::numeric_limits<int>::max(), min_y= std::numeric_limits<int>::max();
				int max_x= std::numeric_limits<int>::min() , max_y=std::numeric_limits<int>::min();
				g.stroke_commands.emplace_back(bgi_font::command{true, sdl::point(x, y)});
				do {
					/* get mask */
					m = b[0] >> 7;
					m += m + (b[1] >> 7);
					/* 0 - stop; 1 - scan; 2 - move; 3 - draw */
					if (m > 1) {
						/* bit arithmetic (note dx/dy signed 8 bit type):
						if sign bit is set - expand to full signed value */
						char dx = (b[0] & 0x7F) | ((b[0] & 0x40) << 1);
						char dy = (b[1] & 0x7F) | ((b[1] & 0x40) << 1);
						dy = -dy;
						/* dx/dy relative to initial screen positon */
						xc = x + dx;
						yc = y + dy;
						if (m == 2) {
							g.stroke_commands.emplace_back(bgi_font::command{true, sdl::point(xc, yc)});
							pen.x = xc;
							pen.y = yc;
						} else {
							g.stroke_commands.emplace_back(bgi_font::command{false, sdl::point(xc, yc)});
							min_x = std::min(min_x, std::min(pen.x, xc));
							min_y = std::min(min_y, std::min(pen.y, yc));

							max_x = std::max(max_x, std::max(pen.x, xc));
							max_y = std::max(max_y, std::max(pen.y, yc));

							pen.x = xc;
							pen.y = yc;
						}
					}
					is.read(reinterpret_cast <char*>(b), 2);
				} while (m);
				char ch = start_char + static_cast <char>(i);
				g.dimension.w = max_x - min_x;
				g.dimension.h = max_y - min_y;
				out.glyphs[ch] = g;
			}

			return out;
		}
	};

	bgi_font_data_manager::bgi_font_data_manager() {
		register_loader("bgi", std::make_unique<bgi_loader>());
	}
}