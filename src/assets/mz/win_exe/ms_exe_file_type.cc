//
// Created by igor on 6/25/24.
//

#include "ms_exe_file_type.hh"
#include <assets/resources/detail/istream_pos_keeper.hh>
#include "istream_wrapper.hh"

namespace neutrino::assets {
	ms_exe_file_type get_ms_exe_file_type(std::istream& is) {
		detail::istream_pos_keeper keeper(is);

		bsw::istream_wrapper stream(is);

		uint16_t old_dos_magic;
		stream >> old_dos_magic;
		if (old_dos_magic != IMAGE_DOS_SIGNATURE) {
			return NONE;
		}
		stream.advance(26 + 32);
		int32_t lfanew;
		stream >> lfanew;
		auto fsize = stream.size();

		if (lfanew < 0 || lfanew > fsize) {
			return MSDOS;
		}
		// read coff magic
		stream.seek(lfanew);
		uint32_t pe_magic;
		stream >> pe_magic;
		if (pe_magic == IMAGE_NT_SIGNATURE) {
			return PE;
		}
		pe_magic = pe_magic & 0xFFFF;
		if (pe_magic == IMAGE_NE_SIGNATURE) {
			return NE;
		}

		return MSDOS;
	}
}
