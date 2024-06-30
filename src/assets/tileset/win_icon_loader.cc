//
// Created by igor on 6/28/24.
//
#include <algorithm>
#include <cstring>
#include <tuple>
#include "win_icon_loader.hh"
#include "mz/windows_resources_loader.hh"
#include "assets/resources/detail/istream_pos_keeper.hh"
#include <bsw/io/binary_reader.hh>
#include <boost/pfr.hpp>
#include <bsw/byte_order.hh>
#include "mz/win_exe/istream_wrapper.hh"
#include <SDL_image.h>
#include <sdlpp/io.hh>

namespace neutrino::assets {
	struct bitmap_header {
		uint32_t size;
		int32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bit_count;
		uint32_t compression;
		uint32_t size_image;
		int32_t x_pels_per_meter;
		int32_t y_pels_per_meter;
		uint32_t clr_used;
		uint32_t clr_important;
	};

	static constexpr auto BITMAP_HEADER_SIZE = 40;
	static_assert(sizeof(bitmap_header) == BITMAP_HEADER_SIZE);
	static constexpr uint32_t PNG_MAGIC = 0x474e5089;
	using icon_disposition = std::tuple <bitmap_header, std::streampos>;

	static sdl::area_type get_dimensions(
		const std::vector <std::tuple <windows_resource_name, windows_rs_icon_info>>& icons, std::istream& is) {
		detail::istream_pos_keeper keeper(is);
		bsw::io::binary_reader rdr(is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
		sdl::area_type area(0, 0);

		for (const auto& [_, ico] : icons) {
			if (ico.size < BITMAP_HEADER_SIZE) {
				continue;
			}
			is.seekg(ico.offset, std::ios::beg);

			bitmap_header hdr{};
			boost::pfr::for_each_field(hdr, [&rdr](auto& field) {
				rdr >> field;
			});
			unsigned w, h;
			if (hdr.size == PNG_MAGIC) {
				is.seekg(ico.offset, std::ios::beg);
				int32_t header[1 + 1 + 1 + 1];
				is.read(reinterpret_cast <char*>(header), sizeof (header));
				w = bsw::byte_order::from_big_endian(header[2]);
				h = bsw::byte_order::from_big_endian(header[3]);
			} else {
				if (hdr.compression) {
					continue;
				}
				bool negative_order = hdr.height < 0;
				w = hdr.width;
				h = negative_order ? (-hdr.height) : (hdr.height);
				if (h & 1) {
					// total_height should be divisible by 2
					continue;
				}
				h = h / 2;
			}
			area.w += w;
			area.h = std::max(area.h, h);
		}

		return area;
	}

	struct rgb_quad {
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t reserved;
	};

	inline uint32_t simple_vec(const std::vector <uint8_t>& data, uint32_t ofs, uint8_t size) {
		switch (size) {
			case 1:
				return (data[ofs / 8] >> (7 - ofs % 8)) & 1;
			case 2:
				return (data[ofs / 4] >> ((3 - ofs % 4) << 1)) & 3;
			case 4:
				return (data[ofs / 2] >> ((1 - ofs % 2) << 2)) & 15;
			case 8:
				return data[ofs];
			case 16:
				return data[2 * ofs] | data[2 * ofs + 1] << 8;
			case 24:
				return data[3 * ofs] | data[3 * ofs + 1] << 8 | data[3 * ofs + 2] << 16;
			case 32:
				return data[4 * ofs] | data[4 * ofs + 1] << 8 | data[4 * ofs + 2] << 16 | data[4 * ofs + 3] << 24;
		}

		return 0;
	}

	static bool load_icon(std::vector <unsigned char>& out, unsigned& w, unsigned& h,
	                      std::streampos offs, uint32_t size, bsw::istream_wrapper& is) {
		if (size < BITMAP_HEADER_SIZE) {
			return false;
		}
		std::streampos has_bytes = 0;

		is.seek(offs);
		bitmap_header bi{};
		is >> bi.size;
		if (bi.size == PNG_MAGIC) {
			// Vista icons
			is.seek(offs);
			out.resize(size);
			is.read(reinterpret_cast <char*>(out.data()), size);
			w = 0;
			h = 0;
		} else {
			is >> bi.width >> bi.height >> bi.planes
				>> bi.bit_count >> bi.compression
				>> bi.size_image >> bi.x_pels_per_meter
				>> bi.y_pels_per_meter >> bi.clr_used >> bi.clr_important;
			if (bi.compression) {
				return false;
			}
			bool negative_order = bi.height < 0;
			const uint32_t width = bi.width;
			const uint32_t total_height = negative_order ? (-bi.height) : (bi.height);

			if (total_height & 1) {
				// total_height should be divisible by 2
				return false;
			}
			const uint32_t height = total_height / 2;

			w = width;
			h = height;

			has_bytes += BITMAP_HEADER_SIZE;
			if (bi.size != BITMAP_HEADER_SIZE) {
				if (bi.size < BITMAP_HEADER_SIZE) {
					return false;
				}
				const std::streampos delta = bi.size - BITMAP_HEADER_SIZE;
				if (delta + has_bytes > size) {
					return false;
				}
				is.advance(bi.size - BITMAP_HEADER_SIZE);
				has_bytes += delta;
			}

			rgb_quad pal[256];
			unsigned int palette_count = 0;
			if (bi.clr_used != 0 || bi.bit_count < 24) {
				palette_count = (bi.clr_used != 0 ? bi.clr_used : 1 << bi.bit_count);
				if (palette_count > 256) {
					return false;
				}
				const std::streampos to_read = palette_count * sizeof(rgb_quad);
				if (has_bytes + to_read > size) {
					return false;
				}
				is.read(reinterpret_cast <char*>(pal), static_cast <size_t>(to_read));
				has_bytes += to_read;
			}
#define ROW_BYTES(bits) ((((bits) + 31) >> 5) << 2)
			const uint32_t row_size = ROW_BYTES(width * bi.bit_count);
			const uint32_t image_size = height * row_size;
			const uint32_t mask_size = height * ROW_BYTES(width);
			has_bytes += (static_cast <std::streampos>(image_size) + static_cast <std::streampos>(mask_size));
			if (has_bytes > size) {
				return false;
			}
			out.resize(width * height * 4);

			std::vector <uint8_t> image_data(image_size);
			std::vector <uint8_t> mask_data(mask_size);
			is.read(reinterpret_cast <char*>(image_data.data()), image_data.size());
			is.read(reinterpret_cast <char*>(mask_data.data()), mask_data.size());

			auto* row = out.data();

			for (uint32_t d = 0; d < height; d++) {
				uint32_t x;
				uint32_t y = (bi.height < 0 ? d : height - d - 1);
				uint32_t imod = y * (image_size / height) * 8 / bi.bit_count;
				uint32_t mmod = y * (mask_size / height) * 8;

				for (x = 0; x < width; x++) {
					uint32_t color = simple_vec(image_data, x + imod, static_cast <uint8_t>(bi.bit_count));

					if (bi.bit_count <= 16) {
						if (color >= palette_count) {
							return false;
						}
						row[4 * x + 0] = pal[color].red;
						row[4 * x + 1] = pal[color].green;
						row[4 * x + 2] = pal[color].blue;
					} else {
						row[4 * x + 0] = (color >> 16) & 0xFF;
						row[4 * x + 1] = (color >> 8) & 0xFF;
						row[4 * x + 2] = (color >> 0) & 0xFF;
					}
					if (bi.bit_count == 32) {
						row[4 * x + 3] = static_cast <uint8_t>((color >> 24) & 0xFF);
					} else {
						row[4 * x + 3] = simple_vec(mask_data, x + mmod, 1) ? 0 : 0xFF;
					}
				}
				row += (4 * width);
			}
		}
		return true;
	}

	static sdl::rect load_ico(sdl::surface& srf, const windows_rs_icon_info& ico, std::istream& is, int px) {
		is.seekg(0, std::ios::beg);
		bsw::istream_wrapper istream_wrapper(is);

		std::vector <unsigned char> out;
		unsigned w;
		unsigned h;

		if (!load_icon(out, w, h, ico.offset, ico.size, istream_wrapper)) {
			return {0, 0};
		}

		if (w > 0 && h > 0) {
			for (unsigned vy = 0; vy < h; vy++) {
				void* dst = static_cast <char*>(srf->pixels) + srf->pitch * vy + 4 * px;
				void* src = out.data() + 4 * w * vy;
				std::memcpy(dst, src, w * 4);
			}
		} else {
			sdl::io rwops(out.data(), out.size());
			sdl::surface s(sdl::object<SDL_Surface>(IMG_LoadPNG_RW(rwops.handle()), true));
			if (s) {
				sdl::rect src_rect(0, 0, static_cast <int>(w), static_cast <int>(h));
				sdl::point p(px, 0);
				auto other = s.convert(sdl::pixel_format(sdl::pixel_format::ARGB8888));
				s.blit(src_rect, srf, p);
			}
		}
		return {px, 0, static_cast <int>(w), static_cast <int>(h)};
	}

	static tileset create_tileset(const sdl::area_type& area,
	                              const std::vector <std::tuple <windows_resource_name, windows_rs_icon_info>>& icons,
	                              std::istream& is) {
		sdl::surface surface(area.w, area.h, sdl::pixel_format(sdl::pixel_format::ARGB8888));
		tileset ts;
		int px = 0;
		tile_id_t tid = 0;

		for (const auto& [_, ico] : icons) {
			auto dims = load_ico(surface, ico, is, px);
			px += dims.w;
			if (dims.w > 0 && dims.h > 0) {
				ts.set_tile(tid++, dims);
			}
		}

		ts.set_surface(std::move(surface));

		return ts;
	}

	win_icon_loader::win_icon_loader()
		: m_exe_loader(std::make_unique <windows_resources_loader>()) {
	}

	bool win_icon_loader::accept(std::istream& is) const {
		if (!m_exe_loader->accept(is)) {
			return false;
		}
		auto rd = m_exe_loader->load(is);
		return rd.exists <windows_rs_icon_info>();
	}

	tileset win_icon_loader::load(std::istream& is) const {
		auto rd = m_exe_loader->load(is);
		auto icons = rd.load <windows_rs_icon_info>();
		auto dims = get_dimensions(icons, is);

		return create_tileset(dims, icons, is);
	}
}
