//
// Created by igor on 6/28/24.
//
#include <algorithm>
#include <tuple>
#include "win_icon_loader.hh"
#include "mz/windows_resources_loader.hh"
#include "assets/resources/detail/istream_pos_keeper.hh"
#include <bsw/io/binary_reader.hh>
#include <boost/pfr.hpp>

namespace neutrino::assets {
#define PNG_MAGIC 0x474e5089

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

	using icon_disposition = std::tuple<bitmap_header, std::streampos>;

	static std::tuple<sdl::area_type, std::vector<icon_disposition>> get_dimensions(
		const std::vector <std::tuple <windows_resource_name, windows_rs_icon_info>>& icons, std::istream& is) {
		detail::istream_pos_keeper keeper(is);
		bsw::io::binary_reader rdr(is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
		sdl::area_type area(0, 0);
		std::vector<icon_disposition> disp;
		for (const auto& [_, ico] : icons) {
			is.seekg(ico.offset, std::ios::beg);

			bitmap_header hdr{};
			boost::pfr::for_each_field(hdr, [&rdr](auto& field) {
				rdr >> field;
			});
			area.w += hdr.width;
			area.h = std::max(area.h, static_cast <unsigned int>(hdr.height));
			disp.emplace_back(hdr, is.tellg());
		}

		return {area, disp};
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
		tileset out;
		auto rd = m_exe_loader->load(is);
		auto icons = rd.load <windows_rs_icon_info>();
		get_dimensions(icons, is);
		return out;
	}
}
