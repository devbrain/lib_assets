#include <algorithm>
#include <boost/pfr.hpp>
#include "windows_pe_file.hh"
#include "abstract_reporter.hh"
#include "mz/win_exe/windows_pe_structs.hh"
#include "mz/win_exe/ms_exe_file_type.hh"
#include "assets/resources/detail/istream_pos_keeper.hh"


static bool is_power_of_two (unsigned int x) {
	return (
		x == 1 || x == 2 || x == 4 || x == 8 || x == 16 || x == 32 ||
		x == 64 || x == 128 || x == 256 || x == 512 || x == 1024 ||
		x == 2048 || x == 4096 || x == 8192 || x == 16384 ||
		x == 32768 || x == 65536 || x == 131072 || x == 262144 ||
		x == 524288 || x == 1048576 || x == 2097152 ||
		x == 4194304 || x == 8388608 || x == 16777216 ||
		x == 33554432 || x == 67108864 || x == 134217728 ||
		x == 268435456 || x == 536870912 || x == 1073741824 ||
		x == 2147483648);
}

namespace neutrino::assets {


	static constexpr uint16_t IMAGE_OPTIONAL_HEADER_PE32 = 0x10B;
	static constexpr uint16_t IMAGE_OPTIONAL_HEADER_PE32_PLUS = 0x20B;

	struct OPTIONAL_HEADER_32 {
		u2 Magic;                   //	The unsigned integer that identifies the state of the image file.The most common number is 0x10B, which identifies it as a normal executable file. 0x107 identifies it as a ROM image, and 0x20B identifies it as a PE32 + executable.
		u1 MajorLinkerVersion;      //	The linker major version number.
		u1 MinorLinkerVersion;         // The linker minor version number.
		u4 SizeOfCode;                 // The size of the code(text) section, or the sum of all code sections if there are multiple sections.
		u4 SizeOfInitializedData;   //	The size of the initialized data section, or the sum of all such sections if there are multiple data sections.
		u4 SizeOfUninitializedData; //	The size of the uninitialized data section(BSS), or the sum of all such sections if there are multiple BSS sections.
		u4 AddressOfEntryPoint;     //	The address of the entry point relative to the image base when the executable file is loaded into memory.For program images, this is the starting address.For device drivers, this is the address of the initialization function.An entry point is optional for DLLs.When no entry point is present, this field must be zero.
		u4 BaseOfCode;              //	The address that is relative to the image base of the beginning-of-code section when it is loaded into memory.
		u4 BaseOfData;              // The address that is relative to the image base of the beginning-of-data section when it is loaded into memory.

		u4 ImageBase;                   //	The preferred address of the first byte of image when loaded into memory; must be a multiple of 64�K.The default for DLLs is 0x10000000. The default for Windows CE EXEs is 0x00010000. The default for Windows�NT, Windows 2000, Windows�XP, Windows�95, Windows�98, and Windows�Me is 0x00400000.
		u4 SectionAlignment;            //	The alignment(in bytes) of sections when they are loaded into memory.It must be greater than or equal to FileAlignment.The default is the page size for the architecture.
		u4 FileAlignment;               //	The alignment factor(in bytes) that is used to align the raw data of sections in the image file.The value should be a power of 2 between 512 and 64�K, inclusive.The default is 512. If the SectionAlignment is less than the architecture�s page size, then FileAlignment must match SectionAlignment.
		u2 MajorOperatingSystemVersion; //	The major version number of the required operating system.
		u2 MinorOperatingSystemVersion; //	The minor version number of the required operating system.
		u2 MajorImageVersion;           //	The major version number of the image.
		u2 MinorImageVersion;           //	The minor version number of the image.
		u2 MajorSubsystemVersion;       //	The major version number of the subsystem.
		u2 MinorSubsystemVersion;       //	The minor version number of the subsystem.
		u4 Win32VersionValue;           //	Reserved, must be zero.
		u4 SizeOfImage;                 //	The size(in bytes) of the image, including all headers, as the image is loaded in memory.It must be a multiple of SectionAlignment.
		u4 SizeOfHeaders;               //	The combined size of an MSDOS stub, PE header, and section headers rounded up to a multiple of FileAlignment.
		u4 CheckSum;                    //	The image file checksum.The algorithm for computing the checksum is incorporated into IMAGHELP.DLL.The following are checked for validation at load time : all drivers, any DLL loaded at boot time, and any DLL that is loaded into a critical Windows process.
		u2 Subsystem;                     // The subsystem that is required to run this image. see enum IMAGE_SUBSYSTEM
		u2 DllCharacteristics;          //	For more information, see enum IMAGE_DLLCHARACTERISTICS.
		u4 SizeOfStackReserve;             // The size of the stack to reserve.Only SizeOfStackCommit is committed; the rest is made available one page at a time until the reserve size is reached.
		u4 SizeOfStackCommit;           //	The size of the stack to commit.
		u4 SizeOfHeapReserve;           //	The size of the local heap space to reserve.Only SizeOfHeapCommit is committed; the rest is made available one page at a time until the reserve size is reached.
		u4 SizeOfHeapCommit;            //	The size of the local heap space to commit.
		u4 LoaderFlags;                 //	Reserved, must be zero.
		u4 NumberOfRvaAndSizes;         //	The number of data - directory entries in the remainder of the optional header.Each describes a location and size.
	};

	static_assert (sizeof (OPTIONAL_HEADER_32) == 96, "OPTIONAL_HEADER_32 size should be 96 bytes");
	// ======================================================================================
	struct OPTIONAL_HEADER_64 {
		u2 Magic;                   //	The unsigned integer that identifies the state of the image file.The most common number is 0x10B, which identifies it as a normal executable file. 0x107 identifies it as a ROM image, and 0x20B identifies it as a PE32 + executable.
		u1 MajorLinkerVersion;      //	The linker major version number.
		u1 MinorLinkerVersion;         // The linker minor version number.
		u4 SizeOfCode;                 // The size of the code(text) section, or the sum of all code sections if there are multiple sections.
		u4 SizeOfInitializedData;   //	The size of the initialized data section, or the sum of all such sections if there are multiple data sections.
		u4 SizeOfUninitializedData; //	The size of the uninitialized data section(BSS), or the sum of all such sections if there are multiple BSS sections.
		u4 AddressOfEntryPoint;     //	The address of the entry point relative to the image base when the executable file is loaded into memory.For program images, this is the starting address.For device drivers, this is the address of the initialization function.An entry point is optional for DLLs.When no entry point is present, this field must be zero.
		u4 BaseOfCode;              //	The address that is relative to the image base of the beginning - of - code section when it is loaded into memory.

		u8 ImageBase;                   //	The preferred address of the first byte of image when loaded into memory; must be a multiple of 64�K.The default for DLLs is 0x10000000. The default for Windows CE EXEs is 0x00010000. The default for Windows�NT, Windows 2000, Windows�XP, Windows�95, Windows�98, and Windows�Me is 0x00400000.
		u4 SectionAlignment;            //	The alignment(in bytes) of sections when they are loaded into memory.It must be greater than or equal to FileAlignment.The default is the page size for the architecture.
		u4 FileAlignment;               //	The alignment factor(in bytes) that is used to align the raw data of sections in the image file.The value should be a power of 2 between 512 and 64�K, inclusive.The default is 512. If the SectionAlignment is less than the architecture�s page size, then FileAlignment must match SectionAlignment.
		u2 MajorOperatingSystemVersion; //	The major version number of the required operating system.
		u2 MinorOperatingSystemVersion; //	The minor version number of the required operating system.
		u2 MajorImageVersion;           //	The major version number of the image.
		u2 MinorImageVersion;           //	The minor version number of the image.
		u2 MajorSubsystemVersion;       //	The major version number of the subsystem.
		u2 MinorSubsystemVersion;       //	The minor version number of the subsystem.
		u4 Win32VersionValue;           //	Reserved, must be zero.
		u4 SizeOfImage;                 //	The size(in bytes) of the image, including all headers, as the image is loaded in memory.It must be a multiple of SectionAlignment.
		u4 SizeOfHeaders;               //	The combined size of an MSDOS stub, PE header, and section headers rounded up to a multiple of FileAlignment.
		u4 CheckSum;                    //	The image file checksum.The algorithm for computing the checksum is incorporated into IMAGHELP.DLL.The following are checked for validation at load time : all drivers, any DLL loaded at boot time, and any DLL that is loaded into a critical Windows process.
		u2 Subsystem;                     // The subsystem that is required to run this image.For more information, see �Windows Subsystem� later in this specification.
		u2 DllCharacteristics;          //	For more information, see �DLL Characteristics� later in this specification.
		u8 SizeOfStackReserve;             // The size of the stack to reserve.Only SizeOfStackCommit is committed; the rest is made available one page at a time until the reserve size is reached.
		u8 SizeOfStackCommit;           //	The size of the stack to commit.
		u8 SizeOfHeapReserve;           //	The size of the local heap space to reserve.Only SizeOfHeapCommit is committed; the rest is made available one page at a time until the reserve size is reached.
		u8 SizeOfHeapCommit;            //	The size of the local heap space to commit.
		u4 LoaderFlags;                 //	Reserved, must be zero.
		u4 NumberOfRvaAndSizes;         //	The number of data - directory entries in the remainder of the optional header.Each describes a location and size.
	};
	static_assert (sizeof (OPTIONAL_HEADER_64) == 112, "OPTIONAL_HEADER_64 size should be 112 bytes");

	// ===========================================================================
	static void assign (OPTIONAL_HEADER& opt, const OPTIONAL_HEADER_32* x) {
		opt.Is64Bit = false;

		opt.MajorLinkerVersion = x->MajorLinkerVersion;
		opt.MinorLinkerVersion = x->MinorLinkerVersion;
		opt.SizeOfCode = x->SizeOfCode;
		opt.SizeOfInitializedData = x->SizeOfInitializedData;
		opt.SizeOfUninitializedData = x->SizeOfUninitializedData;
		opt.AddressOfEntryPoint = x->AddressOfEntryPoint;
		opt.BaseOfCode = x->BaseOfCode;
		opt.BaseOfData = x->BaseOfData;
		opt.ImageBase = x->ImageBase;
		opt.SectionAlignment = x->SectionAlignment;
		opt.FileAlignment = x->FileAlignment;
		opt.MajorOperatingSystemVersion = x->MajorOperatingSystemVersion;
		opt.MinorOperatingSystemVersion = x->MinorOperatingSystemVersion;
		opt.MajorImageVersion = x->MajorImageVersion;
		opt.MinorImageVersion = x->MinorImageVersion;
		opt.MajorSubsystemVersion = x->MajorSubsystemVersion;
		opt.MinorSubsystemVersion = x->MinorSubsystemVersion;
		opt.Win32VersionValue = x->Win32VersionValue;
		opt.SizeOfImage = x->SizeOfImage;
		opt.SizeOfHeaders = x->SizeOfHeaders;
		opt.CheckSum = x->CheckSum;
		opt.Subsystem = x->Subsystem;
		opt.DllCharacteristics = x->DllCharacteristics;
		opt.SizeOfStackReserve = x->SizeOfStackReserve;
		opt.SizeOfStackCommit = x->SizeOfStackCommit;
		opt.SizeOfHeapReserve = x->SizeOfHeapReserve;
		opt.SizeOfHeapCommit = x->SizeOfHeapCommit;
		opt.LoaderFlags = x->LoaderFlags;
		opt.NumberOfRvaAndSizes = x->NumberOfRvaAndSizes;

	}

	static void assign (OPTIONAL_HEADER& opt, const OPTIONAL_HEADER_64* x) {
		opt.Is64Bit = true;

		opt.MajorLinkerVersion = x->MajorLinkerVersion;
		opt.MinorLinkerVersion = x->MinorLinkerVersion;
		opt.SizeOfCode = x->SizeOfCode;
		opt.SizeOfInitializedData = x->SizeOfInitializedData;
		opt.SizeOfUninitializedData = x->SizeOfUninitializedData;
		opt.AddressOfEntryPoint = x->AddressOfEntryPoint;
		opt.BaseOfCode = x->BaseOfCode;
		opt.BaseOfData = 0xFFFFFFFF;
		opt.ImageBase = x->ImageBase;
		opt.SectionAlignment = x->SectionAlignment;
		opt.FileAlignment = x->FileAlignment;
		opt.MajorOperatingSystemVersion = x->MajorOperatingSystemVersion;
		opt.MinorOperatingSystemVersion = x->MinorOperatingSystemVersion;
		opt.MajorImageVersion = x->MajorImageVersion;
		opt.MinorImageVersion = x->MinorImageVersion;
		opt.MajorSubsystemVersion = x->MajorSubsystemVersion;
		opt.MinorSubsystemVersion = x->MinorSubsystemVersion;
		opt.Win32VersionValue = x->Win32VersionValue;
		opt.SizeOfImage = x->SizeOfImage;
		opt.SizeOfHeaders = x->SizeOfHeaders;
		opt.CheckSum = x->CheckSum;
		opt.Subsystem = x->Subsystem;
		opt.DllCharacteristics = x->DllCharacteristics;
		opt.SizeOfStackReserve = x->SizeOfStackReserve;
		opt.SizeOfStackCommit = x->SizeOfStackCommit;
		opt.SizeOfHeapReserve = x->SizeOfHeapReserve;
		opt.SizeOfHeapCommit = x->SizeOfHeapCommit;
		opt.LoaderFlags = x->LoaderFlags;
		opt.NumberOfRvaAndSizes = x->NumberOfRvaAndSizes;

	}



	// ===========================================================================
	void windows_pe_file::_load (abstract_reporter& reporter) {

		bsw::istream_wrapper stream (m_stream);

		uint16_t old_dos_magic;
		stream >> old_dos_magic;
		if (old_dos_magic != IMAGE_DOS_SIGNATURE) {
			RAISE_EX ("Not a MZ file");
		}
		stream.advance (26 + 32);
		int32_t lfanew;
		stream >> lfanew;
		const auto fsize = file_size();

		if (lfanew < 0 || lfanew > fsize ) {
			RAISE_EX ("Not a PE file");
		}
		// read coff magic
		stream.seek (lfanew);
		uint32_t pe_magic;
		stream >> pe_magic;
		if (pe_magic != IMAGE_NT_SIGNATURE) {
			RAISE_EX ("Not a PE file");
		}
		_load_headers (stream, reporter);
		_load_sections (stream, reporter);
	}

	// ----------------------------------------------------------------------------
	windows_pe_file::windows_pe_file (std::istream& is, abstract_reporter& reporter)
		: m_stream (is),
		  m_coff_header {} {
		_load (reporter);
	}
	// -------------------------------------------------------------

	// -------------------------------------------------------------
	// const char* windows_pe_file::read_section (const SECTION& s) const {
	// 	const char* file_data = this->file_data ();
	// 	const std::size_t file_size = this->file_size ();
	// 	const uint32_t rc_offs = translate_rva (s.VirtualAddress);
	//
	// 	if (rc_offs == 0 || (rc_offs + s.SizeOfRawData) > file_size) {
	// 		return nullptr;
	// 	}
	// 	return file_data + rc_offs;
	// }

	// -------------------------------------------------------------
	windows_pe_file::~windows_pe_file () = default;

	// --------------------------------------------------------------
	std::istream& windows_pe_file::stream() const {
		return m_stream;
	}

	// --------------------------------------------------------------
	std::size_t windows_pe_file::file_size () const {
		neutrino::assets::detail::istream_pos_keeper keeper(m_stream);
		m_stream.seekg(0, std::ios::end);
		const auto rc = m_stream.tellg();
		return rc;
	}

	bool windows_pe_file::is_pe() const {
		return true;
	}

	std::size_t windows_pe_file::offset_in_file(uint32_t res_offset) const {
		return translate_rva(res_offset);
	}

	// --------------------------------------------------------------
	std::size_t windows_pe_file::overlay_offset () const {
		const section_t& table = sections ();
		std::size_t offset = 0;
		if (table.empty ()) {
			offset = file_size ();
		}
		for (const SECTION& section : table) {
			u4 aligned_pointer = section.PointerToRawData & ~0x1ff;

			auto endPoint = _get_read_size (section) + aligned_pointer;
			offset = std::max (offset, static_cast <std::size_t>(endPoint));
		}
		if (offset > file_size ()) {
			offset = file_size ();
		}
		return offset;
	}

	// --------------------------------------------------------------
	static u4 alignedUp (u4 value, u4 alignTo) {
		if (value % alignTo != 0) {
			value = (value + alignTo - 1) & ~(alignTo - 1);
		}
		return value;
	}

	// --------------------------------------------------------------
	u4 windows_pe_file::_get_read_size (const SECTION& sec) const {
		auto pointerToRaw = sec.PointerToRawData;
		auto virtSize = sec.VirtualSize;
		auto sizeOfRaw = sec.SizeOfRawData;
		auto fileAlign = optional_header ().FileAlignment;
		auto alignedPointerToRaw = pointerToRaw & ~0x1ff;
		auto readSize = alignedUp (pointerToRaw + sizeOfRaw, fileAlign) - alignedPointerToRaw;
		readSize = std::min (readSize, alignedUp (sizeOfRaw, 0x1000));
		if (virtSize != 0) {
			readSize = std::min (readSize, alignedUp (virtSize, 0x1000));
		}
		return readSize;
	}

	// --------------------------------------------------------------
	uint32_t windows_pe_file::translate_rva (uint32_t rva) const {
		for (const auto& s : m_sections) {
			uint32_t size = s.VirtualSize ? s.VirtualSize : s.SizeOfRawData;
			if (rva >= s.VirtualAddress && rva < s.VirtualAddress + size) {
				return s.PointerToRawData + (rva - s.VirtualAddress);
			}
		}
		return 0;
	}

	// --------------------------------------------------------------
	void windows_pe_file::_load_headers (bsw::istream_wrapper& is, abstract_reporter& reporter) {

		is >> m_coff_header;

		auto optional_header_begin = is.current_pos ();

		uint16_t pe_kind;
		is >> pe_kind;

		is.seek (optional_header_begin);

		if (pe_kind == IMAGE_OPTIONAL_HEADER_PE32) {
			OPTIONAL_HEADER_32 header32 {};
			is >> header32;

			assign (m_optional_header, &header32);
		} else {
			if (pe_kind == IMAGE_OPTIONAL_HEADER_PE32_PLUS) {
				OPTIONAL_HEADER_64 header64{};
				is >> header64;

				assign (m_optional_header, &header64);
			} else {
				reporter.invalid_field_value (abstract_reporter::OPTIONAL_HEADER_MAGIC, pe_kind);
				RAISE_EX ("Unknown type of the OPTIONAL_HEADER");
			}
		}
		m_optional_header.DataDirectory.resize (m_optional_header.NumberOfRvaAndSizes);

		for (auto & i : m_optional_header.DataDirectory) {
			is >> i;
		}
		auto optional_header_end = is.current_pos ();
		auto optional_header_len = optional_header_end - optional_header_begin;

		if (optional_header_len != m_coff_header.SizeOfOptionalHeader) {
			reporter.invalid_struct_length (abstract_reporter::OPTIONAL_HEADER,
											m_coff_header.SizeOfOptionalHeader,
											static_cast<uint32_t>(optional_header_len));
		}
		_check_coff_header (reporter);
	}

	// --------------------------------------------------------------------------------
	void windows_pe_file::_check_coff_header (abstract_reporter& reporter) const {
		if (!detail::check_in_enum<COFF_HEADER::IMAGE_FILE_MACHINE> (m_coff_header.Machine)) {
			reporter.invalid_enum_value (abstract_reporter::COFF_HEADER_MACHINE_TYPE, m_coff_header.Machine);
		}
		if (!detail::check_in_flags<COFF_HEADER::IMAGE_FILE_CHARACTERISTICS> (m_coff_header.Characteristics)) {
			reporter
				.unknown_flag_set (abstract_reporter::COFF_HEADER_CHARACTERISTICS, m_coff_header.Characteristics);
		}
	}

	// --------------------------------------------------------------------------------
	void windows_pe_file::_check_optional_header (abstract_reporter& reporter) const {
		if (m_optional_header.ImageBase % 0xFFFF) {
			reporter.invalid_field_value (abstract_reporter::OPTIONAL_HEADER_IMAGE_BASE, m_optional_header.ImageBase);
		}
		if (!detail::check_in_flags<OPTIONAL_HEADER::IMAGE_SUBSYSTEM> (m_optional_header.Subsystem)) {
			reporter.unknown_flag_set (abstract_reporter::OPTIONAL_HEADER_SUBSYSTEM, m_optional_header.Subsystem);
		}
		if (!detail::check_in_flags<OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS> (m_optional_header.DllCharacteristics)) {
			reporter.unknown_flag_set (abstract_reporter::OPTIONAL_HEADER_DLL_CHARACTERISITCS, m_optional_header
				.DllCharacteristics);
		}
		if (m_optional_header.SectionAlignment < m_optional_header.FileAlignment) {
			reporter.invalid_field_value (abstract_reporter::OPTIONAL_HEADER_SECTION_ALIGNMENT, m_optional_header
				.SectionAlignment);
		}
		if (m_optional_header.FileAlignment < 512 || m_optional_header.FileAlignment > 65536
			|| !is_power_of_two (m_optional_header.FileAlignment)) {
			reporter.invalid_field_value (abstract_reporter::OPTIONAL_HEADER_FILE_ALIGNMENT, m_optional_header
				.FileAlignment);
		}
		if (m_optional_header.Win32VersionValue != 0) {
			reporter.should_be_zero (abstract_reporter::OPTIONAL_HEADER_WIN32VERSIONVALUE, m_optional_header
				.Win32VersionValue);
		}
		if (m_optional_header.SizeOfImage % m_optional_header.SectionAlignment) {
			reporter.invalid_field_value (abstract_reporter::OPTIONAL_HEADER_SIZE_OF_IMAGE, m_optional_header
				.SizeOfImage);
		}
		if (m_optional_header.LoaderFlags != 0) {
			reporter.should_be_zero (abstract_reporter::OPTIONAL_HEADER_LOADER_FLAGS, m_optional_header.LoaderFlags);
		}
	}

	// --------------------------------------------------------------------------------
	void windows_pe_file::_load_sections (bsw::istream_wrapper& stream, abstract_reporter& reporter) {
		for (u2 i = 0; i < m_coff_header.NumberOfSections; i++) {
			SECTION s;
			stream >> s;
			m_sections.push_back (s);
		}
	}

	// --------------------------------------------------------------------------------
	const COFF_HEADER& windows_pe_file::coff_header () const {
		return m_coff_header;
	}

	// --------------------------------------------------------------------------------
	const OPTIONAL_HEADER& windows_pe_file::optional_header () const {
		return m_optional_header;
	}

	// --------------------------------------------------------------------------------
	const windows_pe_file::section_t& windows_pe_file::sections () const {
		return m_sections;
	}



} // ns pefile
