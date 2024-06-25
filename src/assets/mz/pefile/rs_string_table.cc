#include <assets/resources/exe/winres/rs_string_table.hh>
#include "pefile.hh"

namespace assets::pefile {
	int string_table::number () const {
		return m_num;
	}

	// ------------------------------------------------------------------
	string_table::strings_map_t::const_iterator string_table::begin () const {
		return m_strings.begin ();
	}

	// ------------------------------------------------------------------
	string_table::strings_map_t::const_iterator string_table::end () const {
		return m_strings.end ();
	}

	// ------------------------------------------------------------------
	void string_table::_number (int x) {
		m_num = x;
	}

	// ------------------------------------------------------------------
	void string_table::_bind (int id, const std::wstring& s) {
		m_strings[id] = s;
	}

	// ------------------------------------------------------------------
	void string_table::load (const windows_pe_file& file, const resource& rn, string_table& out) {
		const std::size_t file_size = file.file_size ();
		auto offs = rn.offset_in_file (file);

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
			stream >> len;
			has_bytes += sizeof (len);
			if (has_bytes + len > block_len) {
				break;
			}
			if (len != 0) {
				std::wstring w;
				stream.read_string (w, len, false);
				has_bytes += sizeof (wchar_t) * (len);
				out._bind (nums, w);
			}
			nums++;
		}

	}
}