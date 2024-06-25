//
// Created by igor on 6/25/24.
//
#include <iostream>
#include <ctime>
#include <assets/resources/exe/windows_pe.hh>

namespace assets::pefile {
	struct unix_time {
		explicit unix_time(uint32_t t)
			: m_time(t) {
		}

		std::time_t m_time;
	};

	std::ostream& operator<<(std::ostream& os, const unix_time& v) {
		std::tm* ptm = std::localtime(&v.m_time);
		char buffer[32];
		std::strftime(buffer, 32, "%d.%m.%Y %H:%M:%S", ptm);
		os << buffer;
		return os;
	}

	// -------------------------------------------------------------------------------------
	struct as_int {
		explicit as_int(u1 x)
			: m_v(x) {
		}

		u1 m_v;
	};

	std::ostream& operator<<(std::ostream& os, const as_int& v) {
		union {
			u1 b;
			u8 q;
		} u{};
		u.q = 0;
		u.b = v.m_v;
		os << u.q;
		return os;
	}

	// -------------------------------------------------------------------------------------
	template<typename T>
	struct flags {
		explicit flags(uint32_t f)
			: m_flags(f) {
		}

		uint32_t m_flags;
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const flags <T>& v) {
		auto values = detail::flag_traits <T>::data;
		bool first_time = true;
		for (std::size_t i = 0; i < detail::flag_traits <T>::len(); i++) {
			if (v.m_flags & values[i]) {
				if (!first_time) {
					os << ",";
				} else {
					first_time = false;
				}
				os << static_cast <T>(values[i]);
			}
		}
		return os;
	}

	// -------------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, COFF_HEADER::IMAGE_FILE_MACHINE v) {
		switch (v) {
			case COFF_HEADER::IMAGE_FILE_MACHINE_UNKNOWN: os << "IMAGE_FILE_MACHINE_UNKNOWN";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_AM33: os << "IMAGE_FILE_MACHINE_AM33";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_AMD64: os << "IMAGE_FILE_MACHINE_AMD64";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_ARM: os << "IMAGE_FILE_MACHINE_ARM";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_ARMNT: os << "IMAGE_FILE_MACHINE_ARMNT";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_EBC: os << "IMAGE_FILE_MACHINE_EBC";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_I386: os << "IMAGE_FILE_MACHINE_I386";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_IA64: os << "IMAGE_FILE_MACHINE_IA64";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_M32R: os << "IMAGE_FILE_MACHINE_M32R";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_MIPS16: os << "IMAGE_FILE_MACHINE_MIPS16";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_MIPSFPU: os << "IMAGE_FILE_MACHINE_MIPSFPU";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_MIPSFPU16: os << "IMAGE_FILE_MACHINE_MIPSFPU16";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_POWERPC: os << "IMAGE_FILE_MACHINE_POWERPC";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_POWERPCFP: os << "IMAGE_FILE_MACHINE_POWERPCFP";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_R4000: os << "IMAGE_FILE_MACHINE_R4000";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_RISCV32: os << "IMAGE_FILE_MACHINE_RISCV32";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_RISCV64: os << "IMAGE_FILE_MACHINE_RISCV64";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_RISCV128: os << "IMAGE_FILE_MACHINE_RISCV128";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_SH3: os << "IMAGE_FILE_MACHINE_SH3";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_SH3DSP: os << "IMAGE_FILE_MACHINE_SH3DSP";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_SH4: os << "IMAGE_FILE_MACHINE_SH4";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_SH5: os << "IMAGE_FILE_MACHINE_SH5";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_THUMB: os << "IMAGE_FILE_MACHINE_THUMB";
				break;
			case COFF_HEADER::IMAGE_FILE_MACHINE_WCEMIPSV2: os << "IMAGE_FILE_MACHINE_WCEMIPSV2";
				break;
			default: os << "--- UNKNOWN ---";
		}
		return os;
	}

	// -----------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, COFF_HEADER::IMAGE_FILE_CHARACTERISTICS v) {
		switch (v) {
			case COFF_HEADER::IMAGE_FILE_RELOCS_STRIPPED: os << "IMAGE_FILE_RELOCS_STRIPPED";
				break;
			case COFF_HEADER::IMAGE_FILE_EXECUTABLE_IMAGE: os << "IMAGE_FILE_EXECUTABLE_IMAGE";
				break;
			case COFF_HEADER::IMAGE_FILE_LINE_NUMS_STRIPPED: os << "IMAGE_FILE_LINE_NUMS_STRIPPED";
				break;
			case COFF_HEADER::IMAGE_FILE_LOCAL_SYMS_STRIPPED: os << "IMAGE_FILE_LOCAL_SYMS_STRIPPED";
				break;
			case COFF_HEADER::IMAGE_FILE_AGGRESSIVE_WS_TRIM: os << "IMAGE_FILE_AGGRESSIVE_WS_TRIM";
				break;
			case COFF_HEADER::IMAGE_FILE_LARGE_ADDRESS_AWARE: os << "IMAGE_FILE_LARGE_ADDRESS_AWARE";
				break;
			case COFF_HEADER::IMAGE_FILE_RESERVED: os << "IMAGE_FILE_RESERVED";
				break;
			case COFF_HEADER::IMAGE_FILE_BYTES_REVERSED_LO: os << "IMAGE_FILE_BYTES_REVERSED_LO";
				break;
			case COFF_HEADER::IMAGE_FILE_32BIT_MACHINE: os << "IMAGE_FILE_32BIT_MACHINE";
				break;
			case COFF_HEADER::IMAGE_FILE_DEBUG_STRIPPED: os << "IMAGE_FILE_DEBUG_STRIPPED";
				break;
			case COFF_HEADER::IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP: os << "IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP";
				break;
			case COFF_HEADER::IMAGE_FILE_NET_RUN_FROM_SWAP: os << "IMAGE_FILE_NET_RUN_FROM_SWAP";
				break;
			case COFF_HEADER::IMAGE_FILE_SYSTEM: os << "IMAGE_FILE_SYSTEM";
				break;
			case COFF_HEADER::IMAGE_FILE_DLL: os << "IMAGE_FILE_DLL";
				break;
			case COFF_HEADER::IMAGE_FILE_UP_SYSTEM_ONLY: os << "IMAGE_FILE_UP_SYSTEM_ONLY";
				break;
			case COFF_HEADER::IMAGE_FILE_BYTES_REVERSED_HI: os << "IMAGE_FILE_BYTES_REVERSED_HI";
				break;
			default: os << "--- UNKNOWN ---";
		}
		return os;
	}

	// -------------------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, const COFF_HEADER& v) {
		os << "Machine              = " << static_cast <COFF_HEADER::IMAGE_FILE_MACHINE>(v.Machine) << std::endl;
		os << "NumberOfSections     = " << v.NumberOfSections << std::endl;
		os << "TimeDateStamp        = " << unix_time(v.TimeDateStamp) << std::endl;
		os << "PointerToSymbolTable = " << v.PointerToSymbolTable << std::endl;
		os << "NumberOfSymbols      = " << v.NumberOfSymbols << std::endl;
		os << "SizeOfOptionalHeader = " << v.SizeOfOptionalHeader << std::endl;
		os << "Characteristics      = " << flags <COFF_HEADER::IMAGE_FILE_CHARACTERISTICS>(v.Characteristics)
			<< std::endl;
		return os;
	}

	// -------------------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, OPTIONAL_HEADER::IMAGE_SUBSYSTEM v) {
		switch (v) {
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_UNKNOWN: os << "IMAGE_SUBSYSTEM_UNKNOWN";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_NATIVE: os << "IMAGE_SUBSYSTEM_NATIVE";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_WINDOWS_GUI: os << "IMAGE_SUBSYSTEM_WINDOWS_GUI";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_WINDOWS_CUI: os << "IMAGE_SUBSYSTEM_WINDOWS_CUI";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_POSIX_CUI: os << "IMAGE_SUBSYSTEM_POSIX_CUI";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_WINDOWS_CE_GUI: os << "IMAGE_SUBSYSTEM_WINDOWS_CE_GUI";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_APPLICATION: os << "IMAGE_SUBSYSTEM_EFI_APPLICATION";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
				os << "IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER: os << "IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER";
				break;
			case OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_ROM: os << "IMAGE_SUBSYSTEM_EFI_ROM";
				break;
			default: os << "--- UNKNOWN ---";
		}
		return os;
	}

	// ----------------------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS v) {
		switch (v) {
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED1: os << "IMAGE_DLLCHARACTERISTICS_RESERVED1";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED2: os << "IMAGE_DLLCHARACTERISTICS_RESERVED2";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED3: os << "IMAGE_DLLCHARACTERISTICS_RESERVED3";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED4: os << "IMAGE_DLLCHARACTERISTICS_RESERVED4";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA:
				os << "IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE: os << "IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY:
				os << "IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NX_COMPAT: os << "IMAGE_DLLCHARACTERISTICS_NX_COMPAT";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NO_ISOLATION: os << "IMAGE_DLLCHARACTERISTICS_NO_ISOLATION";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NO_SEH: os << "IMAGE_DLLCHARACTERISTICS_NO_SEH";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NO_BIND: os << "IMAGE_DLLCHARACTERISTICS_NO_BIND";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_APPCONTAINER: os << "IMAGE_DLLCHARACTERISTICS_APPCONTAINER";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_WDM_DRIVER: os << "IMAGE_DLLCHARACTERISTICS_WDM_DRIVER";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_GUARD_CF: os << "IMAGE_DLLCHARACTERISTICS_GUARD_CF";
				break;
			case OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE:
				os << "IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE";
				break;
			default: os << "--- UNKNOWN ---";
		}
		return os;
	}

	// -------------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, const OPTIONAL_HEADER& v) {
		os << "Is64Bit =                     " << v.Is64Bit << std::endl;
		os << "MajorLinkerVersion =          " << as_int(v.MajorLinkerVersion) << std::endl;
		os << "MinorLinkerVersion =          " << as_int(v.MinorLinkerVersion) << std::endl;
		os << "SizeOfCode =                  " << v.SizeOfCode << std::endl;
		os << "SizeOfInitializedData =       " << v.SizeOfInitializedData << std::endl;
		os << "SizeOfUninitializedData =     " << v.SizeOfUninitializedData << std::endl;
		os << "AddressOfEntryPoint =         " << v.AddressOfEntryPoint << std::endl;
		os << "BaseOfCode =                  " << v.BaseOfCode << std::endl;
		if (!v.Is64Bit) {
			os << "BaseOfData =                  " << v.BaseOfData << std::endl;
		}
		os << "ImageBase =                   " << v.ImageBase << std::endl;
		os << "SectionAlignment =            " << v.SectionAlignment << std::endl;
		os << "FileAlignment =               " << v.FileAlignment << std::endl;
		os << "MajorOperatingSystemVersion = " << v.MajorOperatingSystemVersion << std::endl;
		os << "MinorOperatingSystemVersion = " << v.MinorOperatingSystemVersion << std::endl;
		os << "MajorImageVersion =           " << v.MajorImageVersion << std::endl;
		os << "MinorImageVersion =           " << v.MinorImageVersion << std::endl;
		os << "MajorSubsystemVersion =       " << v.MajorSubsystemVersion << std::endl;
		os << "MinorSubsystemVersion =       " << v.MinorSubsystemVersion << std::endl;
		os << "Win32VersionValue =           " << v.Win32VersionValue << std::endl;
		os << "SizeOfImage =                 " << v.SizeOfImage << std::endl;
		os << "SizeOfHeaders =               " << v.SizeOfHeaders << std::endl;
		os << "CheckSum =                    " << v.CheckSum << std::endl;
		os << "Subsystem =                   " << static_cast <OPTIONAL_HEADER::IMAGE_SUBSYSTEM>(v.Subsystem)
			<< std::endl;
		os << "DllCharacteristics =          "
			<< flags <OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS>(v.DllCharacteristics) << std::endl;
		os << "SizeOfStackReserve =          " << v.SizeOfStackReserve << std::endl;
		os << "SizeOfStackCommit =           " << v.SizeOfStackCommit << std::endl;
		os << "SizeOfHeapReserve =           " << v.SizeOfHeapReserve << std::endl;
		os << "SizeOfHeapCommit =            " << v.SizeOfHeapCommit << std::endl;
		os << "LoaderFlags =                 " << v.LoaderFlags << std::endl;
		os << "NumberOfRvaAndSizes =         " << v.NumberOfRvaAndSizes << std::endl;
		return os;
	}

	// ================================================================================
	SECTION_NAME::SECTION_NAME()
		: name{0} {
	}

	// --------------------------------------------------------------------------------
	SECTION_NAME::SECTION_NAME(u8 a)
		: name{a} {
	}

	// --------------------------------------------------------------------------------
	std::string SECTION_NAME::to_string() const {
		if (name.Str[7] == 0) {
			return name.Str;
		}
		return {name.Str, name.Str + 7};
	}

	// --------------------------------------------------------------------------------
	u8 SECTION_NAME::qword() const {
		return name.Qword;
	}

	// --------------------------------------------------------------------------------
	bool operator<(const SECTION_NAME& a, const SECTION_NAME& b) {
		return a.qword() < b.qword();
	}

	// --------------------------------------------------------------------------------
	bool operator==(const SECTION_NAME& a, const SECTION_NAME& b) {
		return a.qword() == b.qword();
	}

	// --------------------------------------------------------------------------------
	bool operator!=(const SECTION_NAME& a, const SECTION_NAME& b) {
		return !(a == b);
	}

	// --------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, const SECTION_NAME& s) {
		os << s.to_string();
		return os;
	}

	// --------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, SECTION::CHARACTERISITICS v) {
		switch (v) {
			case SECTION::IMAGE_SCN_TYPE_NO_PAD: os << "IMAGE_SCN_TYPE_NO_PAD";
				break;
			case SECTION::IMAGE_SCN_CNT_CODE: os << "IMAGE_SCN_CNT_CODE";
				break;
			case SECTION::IMAGE_SCN_CNT_INITIALIZED_DATA: os << "IMAGE_SCN_CNT_INITIALIZED_DATA";
				break;
			case SECTION::IMAGE_SCN_CNT_UNINITIALIZED_DATA: os << "IMAGE_SCN_CNT_UNINITIALIZED_DATA";
				break;
			case SECTION::IMAGE_SCN_LNK_OTHER: os << "IMAGE_SCN_LNK_OTHER";
				break;
			case SECTION::IMAGE_SCN_LNK_INFO: os << "IMAGE_SCN_LNK_INFO";
				break;
			case SECTION::IMAGE_SCN_LNK_REMOVE: os << "IMAGE_SCN_LNK_REMOVE";
				break;
			case SECTION::IMAGE_SCN_LNK_COMDAT: os << "IMAGE_SCN_LNK_COMDAT";
				break;
			case SECTION::IMAGE_SCN_GPREL: os << "IMAGE_SCN_GPREL";
				break;
			case SECTION::IMAGE_SCN_ALIGN_1BYTES: os << "IMAGE_SCN_ALIGN_1BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_2BYTES: os << "IMAGE_SCN_ALIGN_2BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_4BYTES: os << "IMAGE_SCN_ALIGN_4BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_8BYTES: os << "IMAGE_SCN_ALIGN_8BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_16BYTES: os << "IMAGE_SCN_ALIGN_16BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_32BYTES: os << "IMAGE_SCN_ALIGN_32BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_64BYTES: os << "IMAGE_SCN_ALIGN_64BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_128BYTES: os << "IMAGE_SCN_ALIGN_128BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_256BYTES: os << "IMAGE_SCN_ALIGN_256BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_512BYTES: os << "IMAGE_SCN_ALIGN_512BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_1024BYTES: os << "IMAGE_SCN_ALIGN_1024BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_2048BYTES: os << "IMAGE_SCN_ALIGN_2048BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_4096BYTES: os << "IMAGE_SCN_ALIGN_4096BYTES";
				break;
			case SECTION::IMAGE_SCN_ALIGN_8192BYTES: os << "IMAGE_SCN_ALIGN_8192BYTES";
				break;
			case SECTION::IMAGE_SCN_LNK_NRELOC_OVFL: os << "IMAGE_SCN_LNK_NRELOC_OVFL";
				break;
			case SECTION::IMAGE_SCN_MEM_DISCARDABLE: os << "IMAGE_SCN_MEM_DISCARDABLE";
				break;
			case SECTION::IMAGE_SCN_MEM_NOT_CACHED: os << "IMAGE_SCN_MEM_NOT_CACHED";
				break;
			case SECTION::IMAGE_SCN_MEM_NOT_PAGED: os << "IMAGE_SCN_MEM_NOT_PAGED";
				break;
			case SECTION::IMAGE_SCN_MEM_SHARED: os << "IMAGE_SCN_MEM_SHARED";
				break;
			case SECTION::IMAGE_SCN_MEM_EXECUTE: os << "IMAGE_SCN_MEM_EXECUTE";
				break;
			case SECTION::IMAGE_SCN_MEM_READ: os << "IMAGE_SCN_MEM_READ";
				break;
			case SECTION::IMAGE_SCN_MEM_WRITE: os << "IMAGE_SCN_MEM_WRITE";
				break;
		}
		return os;
	}

	// ---------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, const SECTION& v) {
		os << "Name                 = " << v.Name << std::endl;
		os << "VirtualSize          = " << v.VirtualSize << std::endl;
		os << "VirtualAddress       = " << v.VirtualAddress << std::endl;
		os << "SizeOfRawData        = " << v.SizeOfRawData << std::endl;
		os << "PointerToRawData     = " << v.PointerToRawData << std::endl;
		os << "PointerToRelocations = " << v.PointerToRelocations << std::endl;
		os << "PointerToLinenumbers = " << v.PointerToLinenumbers << std::endl;
		os << "NumberOfRelocations  = " << v.NumberOfRelocations << std::endl;
		os << "NumberOfLinenumbers  = " << v.NumberOfLinenumbers << std::endl;
		os << "Characteristics      = " << flags <SECTION::CHARACTERISITICS>(v.Characteristics) << std::endl;

		return os;
	}
}
