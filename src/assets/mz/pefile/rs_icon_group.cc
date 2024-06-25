#include <assets/resources/exe/winres/rs_icon_group.hh>
#include "istream_wrapper.hh"
#include "pefile.hh"

namespace assets::pefile {
	void icon_group::_add_group (uint32_t group_id, uint16_t ordinal) {
		m_grp.insert (icon_group_t::value_type (group_id, ordinal));
	}

	// ------------------------------------------------------------------
	icon_group::icon_group_t::const_iterator icon_group::begin () const {
		return m_grp.begin ();
	}

	// ------------------------------------------------------------------
	icon_group::icon_group_t::const_iterator icon_group::end () const {
		return m_grp.end ();
	}
	// ------------------------------------------------------------------
	namespace {
		struct icon_grp_header_s {
			explicit icon_grp_header_s (bsw::istream_wrapper& is)
				: wReserved (0), wType (0), wCount (0) {
				is >> wReserved;
				is >> wType;
				is >> wCount;
			}

			uint16_t wReserved;          /* Currently zero  */
			uint16_t wType;              /* 1 for icons */
			uint16_t wCount;             /* Number of components */
		};
	}

	// ------------------------------------------------------------------
	void icon_group::load (const windows_pe_file& file, const resource& rn, icon_group& out) {
		const std::size_t file_size = file.file_size ();
		auto offs = rn.offset_in_file (file);

		if (offs >= file_size) {
			return;
		}
		bsw::istream_wrapper stream (file.stream(), offs, rn.size ());

		icon_grp_header_s hdr (stream);
		if (hdr.wType != 1) {
			return;
		}
		for (uint16_t i = 0; i < hdr.wCount; i++) {
			stream.advance (4 * sizeof (uint8_t) + 2 * sizeof (uint16_t) + sizeof (uint32_t));
			uint16_t ordinal;
			stream >> ordinal;
			out._add_group (rn.name ().id (), ordinal);
		}
	}

	// ===================================================================
	icon_info::icon_info (uint16_t id, uint64_t offs, uint32_t s)
		: ordinal (id),
		  offset (offs),
		  size (s) {
	}

	// ------------------------------------------------------------------
	icon_info::icon_info ()
		: ordinal (0xFFFF),
		  offset (0),
		  size (0) {

	}

	// ------------------------------------------------------------------
	void icon_info::load (const windows_pe_file& file, const resource& rn, icon_info& out) {
		out.ordinal = 0xFFFF;
		if (rn.name ().is_id ()) {
			out.ordinal = rn.name ().id ();
		}

		out.offset = rn.offset_in_file (file);
		out.size = rn.size ();
	}
} // ns pefile
