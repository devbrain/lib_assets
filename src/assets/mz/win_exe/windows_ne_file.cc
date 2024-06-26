//
// Created by igor on 6/26/24.
//

#include <cstdint>
#include <vector>
#include "windows_ne_file.hh"
#include "istream_wrapper.hh"
#include "ms_exe_file_type.hh"
#include "pe_resource_builder.hh"
#include <bsw/strings/wchar.hh>
#include <assets/resources/detail/istream_pos_keeper.hh>

#define ID_MASK                             0x7FFF
#define TYPE_MASK                           0xFF

namespace neutrino::assets {
	static
	bsw::istream_wrapper& operator >>(bsw::istream_wrapper& is, tname_info& x) {
		is >> x.rnOffset >> x.rnLength >> x.rnFlags >> x.rnID >> x.rnHandle >> x.rnUsage;
		return is;
	}

	static
	bsw::istream_wrapper& operator >>(bsw::istream_wrapper& is, type_info& x) {
		is >> x.rtTypeID;
		if (x.rtTypeID == 0) {
			return is;
		}
		is >> x.rtResourceCount >> x.rtReserved;

		return is;
	}

	inline uint16_t translate_id(uint16_t x) {
		if (x > 0x8000) {
			return x & ID_MASK;
		}
		return x;
	}

	static std::string read_string(std::istream& is, uint32_t offs) {
		neutrino::assets::detail::istream_pos_keeper keeper(is);
		if (!is.seekg(offs, std::ios::beg)) {
			is.seekg(keeper.curr, std::ios::beg);
		}
		uint8_t length;
		is.read((char*)&length, 1);
		std::string out;
		for (unsigned i = 0; i < length; i++) {
			char b;
			is.read((char*)&b, 1);
			out += b;
		}
		return out;
	}

	windows_ne_file::windows_ne_file(std::istream& stream)
		: m_stream(stream) {
		bsw::istream_wrapper is(stream);

		std::streampos fsize = is.size_to_end();
		m_file_size = fsize;

		uint16_t old_dos_magic;
		is >> old_dos_magic;
		if (old_dos_magic != IMAGE_DOS_SIGNATURE) {
			throw std::runtime_error("Not a MZ file");
		}
		is.advance(26 + 32);
		int32_t lfanew;
		is >> lfanew;
		if (lfanew < 0 || lfanew > fsize) {
			throw std::runtime_error("Not a PE file");
		}
		// read coff magic
		is.seek(lfanew);
		uint16_t ne_magic;
		is >> ne_magic;
		if (ne_magic != IMAGE_NE_SIGNATURE) {
			throw std::runtime_error("Not a PE file");
		}

		is.advance(34);
		uint16_t resident_tab;
		uint16_t res_table_offset;
		is >> res_table_offset >> resident_tab;
		if (res_table_offset == resident_tab) {
			throw std::runtime_error("No resources found");
		}
		uint32_t rc_offset = lfanew + res_table_offset;
		is.seek(rc_offset);

		is >> m_align_shift;

		while (true) {
			type_info ti;
			is >> ti;
			ti.rtNameInfo.resize(ti.rtResourceCount);
			for (uint16_t i = 0; i < ti.rtResourceCount; i++) {
				is >> ti.rtNameInfo[i];
				if (ti.rtNameInfo[i].rnID & 0x8000) {
					ti.rtNameInfo[i].rnID &= 0x7FFF;
				} else {
					ti.rtNameInfo[i].name = read_string(stream, rc_offset + ti.rtNameInfo[i].rnID);
				}
			}
			if (ti.rtTypeID == 0) {
				break;
			} else {
				if (ti.rtTypeID & 0x8000) {
					ti.rtTypeID &= 0x7FFF;
				} else {
					ti.name = read_string(stream, rc_offset + ti.rtTypeID);
				}
				m_types_info.push_back(ti);
			}
		}
	}

	uint16_t windows_ne_file::get_align_shift() const {
		return m_align_shift;
	}

	const std::vector <type_info>& windows_ne_file::get_types_info() const {
		return m_types_info;
	}

	void windows_ne_file::build_resources(windows_resource_directory& out) const {
		detail::resource_dir_builder builder(out);
		windows_resource_name main_name(0);

		for (const auto& ti : m_types_info) {
			windows_resource_name sub_name(0);
			if (ti.name) {
				sub_name.name(bsw::utf8_to_wstring(*ti.name));
			} else {
				sub_name.id(ti.rtTypeID);
			}
			builder.start_main_entry(sub_name);

			for (std::size_t j = 0; j < ti.rtNameInfo.size(); j++) {
				const auto& ni = ti.rtNameInfo[j];
				uint32_t offs = (1 << m_align_shift) * ni.rnOffset;
				uint16_t ordinal = translate_id(ni.rnID);
				uint32_t size = (1 << m_align_shift) * ni.rnLength;
				windows_resource_name rn;
				if (ni.name) {
					rn.name(bsw::utf8_to_wstring(*ni.name));
				} else {
					rn.id(ni.rnID);
				}
				windows_resource r;
				r.name(rn);
				r.offset(offs);
				r.size(size);
				builder.add_to_sub_entry(r);
			}
			builder.end_main_entry();
		}
	}

	std::istream& windows_ne_file::stream() const {
		return m_stream;
	}

	std::size_t windows_ne_file::file_size() const {
		return m_file_size;
	}

	std::size_t windows_ne_file::offset_in_file(uint32_t res_offset) const {
		return res_offset;
	}
}
