#include <assets/resources/exe/winres/rs_icon_group.hh>
#include "mz/win_exe/istream_wrapper.hh"
#include "mz/win_exe/ms_file.hh"

namespace neutrino::assets {
	void windows_rs_icon_group::_add_group (uint32_t group_id, uint16_t ordinal) {
		m_grp.insert (icon_group_t::value_type (group_id, ordinal));
	}

	// ------------------------------------------------------------------
	windows_rs_icon_group::icon_group_t::const_iterator windows_rs_icon_group::begin () const {
		return m_grp.begin ();
	}

	// ------------------------------------------------------------------
	windows_rs_icon_group::icon_group_t::const_iterator windows_rs_icon_group::end () const {
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
	void windows_resource_traits<windows_rs_icon_group>::load (const ms_file& file, const windows_resource& rn, windows_rs_icon_group& out) {
		const std::size_t file_size = file.file_size ();
		auto offs = file.offset_in_file (rn.offset());

		if (offs >= file_size) {
			return;
		}
		bsw::istream_wrapper stream (file.stream(), offs, rn.size ());

		icon_grp_header_s hdr (stream);
		if (hdr.wType != 1) {
			return;
		}
		for (uint16_t i = 0; i < hdr.wCount; i++) {
			uint8_t   bWidth;               // Width, in pixels, of the image
			uint8_t   bHeight;              // Height, in pixels, of the image
			uint8_t   bColorCount;          // Number of colors in image (0 if >=8bpp)
			uint8_t   bReserved;            // Reserved
			uint16_t   wPlanes;              // Color Planes
			uint16_t   wBitCount;            // Bits per pixel
			uint32_t   dwBytesInRes;         // how many bytes in this resource?
			uint16_t   nID;                  // the ID
			//stream.advance (4 * sizeof (uint8_t) + 2 * sizeof (uint16_t) + sizeof (uint32_t));
			//uint16_t ordinal;
			stream >> bWidth >> bHeight >> bColorCount >> bReserved >> wPlanes >> wBitCount >> dwBytesInRes >> nID;
			out._add_group (rn.name ().id (), nID);
		}
	}

	// ===================================================================
	windows_rs_icon_info::windows_rs_icon_info (uint16_t id, uint64_t offs, uint32_t s)
		: ordinal (id),
		  offset (offs),
		  size (s) {
	}

	// ------------------------------------------------------------------
	windows_rs_icon_info::windows_rs_icon_info ()
		: ordinal (0xFFFF),
		  offset (0),
		  size (0) {

	}

	// ------------------------------------------------------------------
	void windows_resource_traits<windows_rs_icon_info>::load (const ms_file& file, const windows_resource& rn, windows_rs_icon_info& out) {
		out.ordinal = 0xFFFF;
		if (rn.name ().is_id ()) {
			out.ordinal = rn.name ().id ();
		}

		out.offset = file.offset_in_file (rn.offset());
		out.size = rn.size ();
	}
} // ns pefile
