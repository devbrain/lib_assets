//
// Created by igor on 6/25/24.
//

#ifndef  MS_EXE_FILE_TYPE_HH
#define  MS_EXE_FILE_TYPE_HH

#include <iosfwd>
#include <cstdint>

namespace neutrino::assets {
	enum ms_exe_file_type {
		NONE,
		MSDOS,
		NE,
		PE
	};

	static inline constexpr uint16_t IMAGE_DOS_SIGNATURE = 0x5A4D;
	static inline constexpr uint32_t IMAGE_NT_SIGNATURE  = 0x00004550;
	static inline constexpr uint16_t IMAGE_NE_SIGNATURE  = 0x454E;
	ms_exe_file_type get_ms_exe_file_type(std::istream& is);
}


#endif
