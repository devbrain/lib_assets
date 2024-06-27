#include <assets/resources/exe/winres/rs_string_table.hh>
#include "mz/win_exe/ms_file.hh"
#include "mz/win_exe/istream_wrapper.hh"
#include <bsw/strings/wchar.hh>

namespace neutrino::assets {
	int windows_rs_string_table::number () const {
		return m_num;
	}

	// ------------------------------------------------------------------
	windows_rs_string_table::strings_map_t::const_iterator windows_rs_string_table::begin () const {
		return m_strings.begin ();
	}

	// ------------------------------------------------------------------
	windows_rs_string_table::strings_map_t::const_iterator windows_rs_string_table::end () const {
		return m_strings.end ();
	}

	// ------------------------------------------------------------------
	void windows_rs_string_table::_number (int x) {
		m_num = x;
	}

	// ------------------------------------------------------------------
	void windows_rs_string_table::_bind (int id, const std::wstring& s) {
		m_strings[id] = s;
	}

	// ------------------------------------------------------------------
	void windows_resource_traits<windows_rs_string_table>::load (const ms_file& file, const windows_resource& rn, windows_rs_string_table& out) {
		const std::size_t file_size = file.file_size ();
		auto offs = file.offset_in_file (rn.offset());

		if (offs >= file_size) {
			return;
		}

		bsw::istream_wrapper stream (file.stream(), offs, rn.size ());

		const auto id = static_cast<uint16_t>(rn.name ().id ());

		out._number (id);
		int nums = 16 * (id - 1);
		std::size_t has_bytes = 0;
		std::size_t block_len = rn.size ();

		while (has_bytes < block_len) {
			uint16_t len;
			if (file.is_pe()) {
				stream >> len;
				has_bytes += sizeof (len);
			} else {
				uint8_t ne_len;
				stream >> ne_len;
				len = ne_len;
				has_bytes += sizeof (ne_len);
			}

			if (has_bytes + len > block_len) {
				break;
			}
			if (len != 0) {
				if (file.is_pe()) {
					std::wstring w;
					stream.read_string (w, len, false);
					has_bytes += sizeof (uint16_t) * (len);
					out._bind (nums, w);
				} else {
					std::string w;
					stream.read_string (w, len, false);
					has_bytes += len;
					try {
						out._bind (nums, bsw::utf8_to_wstring(w));
					} catch (std::exception& e) {
						std::wstring fallback;
						for (auto c : w) {
							fallback += static_cast<wchar_t>(c);
						}
						out._bind (nums, fallback);
					}
				}
			}
			nums++;
		}

	}
}