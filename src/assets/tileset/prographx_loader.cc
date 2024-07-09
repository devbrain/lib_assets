//
// Created by igor on 7/7/24.
//

#include "prographx_loader.hh"
#include <sdlpp/video/color_names.hh>
#include <assets/resources/image/ega_image.hh>
#include <bsw/io/binary_reader.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {
	bool prographx_loader::accept(const tileset_resource& arg) const {
		if (const auto* res = dynamic_cast <const prographx_resource*>(&arg)) {
			std::istream& is = res->get_stream();
			bsw::io::binary_reader rdr(is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
			auto pos = is.tellg();
			is.seekg(0, std::ios::end);
			auto fsize = static_cast <std::size_t>(is.tellg() - pos);
			is.seekg(pos, std::ios::beg);
			if (fsize <= 3) {
				return false;
			}

			if ((std::size_t)pos + 3 >= fsize) {
				return false;
			}
			uint8_t num_tiles, width_bytes, height;
			rdr >> num_tiles >> width_bytes >> height;
			unsigned int block_length = width_bytes * height * (res->is_masked() ? 5 : 4);
			return block_length <= fsize;
		}
		return false;
	}

	tileset prographx_loader::load(const tileset_resource& arg) const {
		if (const auto* res = dynamic_cast <const prographx_resource*>(&arg)) {
			tileset ts;
			ega_byte_description descr{
				res->is_masked() ? ega_bit_purpose::opaque1 : ega_bit_purpose::unused,
				ega_bit_purpose::red1,
				ega_bit_purpose::blue1,
				ega_bit_purpose::green1,
				ega_bit_purpose::intensity1
			};
			std::istream& is = res->get_stream();
			std::vector <pixels> tiles;
			std::vector <pixels> masks;
			bsw::io::binary_reader rdr(is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
			std::size_t tiles_total = 0;
			bool done = false;
			while (!done) {
				uint8_t num_tiles, width_bytes, height;
				rdr >> num_tiles >> width_bytes >> height;
				if (is.eof()) {
					break;
				}
				for (uint8_t i = 0; i < num_tiles; i++) {
					if (tiles_total < res->get_max_tiles()) {
						auto [tile, mask] = load_byteplanar_ega(is, descr, 8 * width_bytes, height);
						tiles.emplace_back(std::move(tile));
						masks.emplace_back(std::move(mask));
						tiles_total++;
					} else {
						done = true;
						break;
					}
				}
				if (done) {
					break;
				}
				auto padding = static_cast <std::streamoff>(res->get_padding());
				is.seekg(padding, std::ios::cur);
			}
			auto [srf, rects] = render_pixels_to_surface(tiles, sdl::colors::white, sdl::colors::black);
			ts.set_surface(std::move(srf));
			for (std::size_t i = 0; i < rects.size(); i++) {
				ts.set_tile(tile_id_t(i), rects[i], std::move(masks[i]));
			}
			return ts;
		}
		RAISE_EX("Should not be here");
	}
}
