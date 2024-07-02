//
// Created by igor on 6/25/24.
//

#ifndef WINDOWS_PE_HH
#define WINDOWS_PE_HH

#include <cstdint>
#include <iosfwd>
#include <vector>
#include "mz/win_exe/istream_wrapper.hh"

namespace neutrino::assets {
	using u1 = uint8_t;
	using u2 = uint16_t;
	using u4 = uint32_t;
	using u8 = uint64_t;

	struct COFF_HEADER {
		enum IMAGE_FILE_MACHINE {
			IMAGE_FILE_MACHINE_UNKNOWN = 0x0,
			// The contents of this field are assumed to be applicable to any machine type
			IMAGE_FILE_MACHINE_AM33 = 0x1d3, // Matsushita AM33
			IMAGE_FILE_MACHINE_AMD64 = 0x8664, // x64
			IMAGE_FILE_MACHINE_ARM = 0x1c0, // ARM little endian
			IMAGE_FILE_MACHINE_ARMNT = 0x1c4, // ARM Thumb - 2 little endian
			IMAGE_FILE_MACHINE_EBC = 0xebc, // EFI byte code
			IMAGE_FILE_MACHINE_I386 = 0x14c, // Intel 386 or later processors and compatible processors
			IMAGE_FILE_MACHINE_IA64 = 0x200, // Intel Itanium processor family
			IMAGE_FILE_MACHINE_M32R = 0x9041, // Mitsubishi M32R little endian
			IMAGE_FILE_MACHINE_MIPS16 = 0x266, // MIPS16
			IMAGE_FILE_MACHINE_MIPSFPU = 0x366, // MIPS with FPU
			IMAGE_FILE_MACHINE_MIPSFPU16 = 0x466, // MIPS16 with FPU
			IMAGE_FILE_MACHINE_POWERPC = 0x1f0, // Power PC little endian
			IMAGE_FILE_MACHINE_POWERPCFP = 0x1f1, // Power PC with floating point support
			IMAGE_FILE_MACHINE_R4000 = 0x166, // MIPS little endian
			IMAGE_FILE_MACHINE_RISCV32 = 0x5032, // RISC - V 32 - bit address space
			IMAGE_FILE_MACHINE_RISCV64 = 0x5064, // RISC - V 64 - bit address space
			IMAGE_FILE_MACHINE_RISCV128 = 0x5128, // RISC - V 128 - bit address space
			IMAGE_FILE_MACHINE_SH3 = 0x1a2, // Hitachi SH3
			IMAGE_FILE_MACHINE_SH3DSP = 0x1a3, // Hitachi SH3 DSP
			IMAGE_FILE_MACHINE_SH4 = 0x1a6, // Hitachi SH4
			IMAGE_FILE_MACHINE_SH5 = 0x1a8, // Hitachi SH5
			IMAGE_FILE_MACHINE_THUMB = 0x1c2, // Thumb
			IMAGE_FILE_MACHINE_WCEMIPSV2 = 0x169 // MIPS little - endian WCE v2
		};

		u2 Machine; // The number that identifies the type of target machine. see enum IMAGE_FILE_MACHINE
		u2 NumberOfSections;
		// The number of sections. This indicates the size of the section table, which immediately follows the headers.
		u4 TimeDateStamp;
		// The low 32 bits of the number of seconds since 00:00 January 1, 1970 (a C run-time time_t value), that indicates when the file was created.
		u4 PointerToSymbolTable;
		// The file offset of the COFF symbol table, or zero if no COFF symbol table is present. This value should be zero for an image because COFF debugging information is deprecated.
		u4 NumberOfSymbols;
		// The number of entries in the symbol table. This data can be used to locate the string table, which immediately follows the symbol table. This value should be zero for an image because COFF debugging information is deprecated.
		u2 SizeOfOptionalHeader;
		// The size of the optional header, which is required for executable files but not for object files. This value should be zero for an object file. For a description of the header format
		u2 Characteristics; // The flags that indicate the attributes of the file. enum IMAGE_FILE_CHARACTERISTICS

		enum IMAGE_FILE_CHARACTERISTICS {
			IMAGE_FILE_RELOCS_STRIPPED = 0x0001,
			//	Image only, Windows CE, and Microsoft Windows�NT� and later.This indicates that the file does not contain base relocations and must therefore be loaded at its preferred base address.If the base address is not available, the loader reports an error.The default behavior of the linker is to strip base relocations from executable(EXE) files.
			IMAGE_FILE_EXECUTABLE_IMAGE = 0x0002,
			//	Image only.This indicates that the image file is valid and can be run.If this flag is not set, it indicates a linker error.
			IMAGE_FILE_LINE_NUMS_STRIPPED = 0x0004,
			//	COFF line numbers have been removed.This flag is deprecated and should be zero.
			IMAGE_FILE_LOCAL_SYMS_STRIPPED = 0x0008,
			//	COFF symbol table entries for local symbols have been removed.This flag is deprecated and should be zero.
			IMAGE_FILE_AGGRESSIVE_WS_TRIM = 0x0010,
			//	Obsolete.Aggressively trim working set.This flag is deprecated for Windows 2000 and later and must be zero.
			IMAGE_FILE_LARGE_ADDRESS_AWARE = 0x0020, //	Application can handle > 2GB addresses.
			IMAGE_FILE_RESERVED = 0x0040, //	This flag is reserved for future use.
			IMAGE_FILE_BYTES_REVERSED_LO = 0x0080,
			//	Little endian : the least significant bit(LSB) precedes the most significant bit(MSB) in memory.This flag is deprecated and should be zero.
			IMAGE_FILE_32BIT_MACHINE = 0x0100, //	Machine is based on a 32 - bit - word architecture.
			IMAGE_FILE_DEBUG_STRIPPED = 0x0200, //	Debugging information is removed from the image file.
			IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP = 0x0400,
			//	If the image is on removable media, fully load it and copy it to the swap file.
			IMAGE_FILE_NET_RUN_FROM_SWAP = 0x0800,
			//	If the image is on network media, fully load it and copy it to the swap file.
			IMAGE_FILE_SYSTEM = 0x1000, //	The image file is a system file, not a user program.
			IMAGE_FILE_DLL = 0x2000,
			//	The image file is a dynamic - link library(DLL).Such files are considered executable files for almost all purposes, although they cannot be directly run.
			IMAGE_FILE_UP_SYSTEM_ONLY = 0x4000, //	The file should be run only on a uniprocessor machine.
			IMAGE_FILE_BYTES_REVERSED_HI = 0x8000
			//	Big endian : the MSB precedes the LSB in memory.This flag is deprecated and should be zero.
		};
	};

	static_assert(sizeof(COFF_HEADER) == 20, "COFF_HEADER size should be 20 bytes");

	std::ostream& operator<<(std::ostream& os, COFF_HEADER::IMAGE_FILE_MACHINE v);
	std::ostream& operator<<(std::ostream& os, COFF_HEADER::IMAGE_FILE_CHARACTERISTICS v);
	std::ostream& operator<<(std::ostream& os, const COFF_HEADER& v);

	// =========================================================
	struct DATA_DIRECTORY {
		u4 VirtualAddress;
		u4 Size;
	};

	static_assert(sizeof(DATA_DIRECTORY) == 8, "DATA_DIRECTORY size should be 8 bytes");

	// =========================================================
	struct OPTIONAL_HEADER {
		bool Is64Bit{};
		u1 MajorLinkerVersion{}; //	The linker major version number.
		u1 MinorLinkerVersion{}; // The linker minor version number.
		u4 SizeOfCode{};
		// The size of the code(text) section, or the sum of all code sections if there are multiple sections.
		u4 SizeOfInitializedData{};
		//	The size of the initialized data section, or the sum of all such sections if there are multiple data sections.
		u4 SizeOfUninitializedData{};
		//	The size of the uninitialized data section(BSS), or the sum of all such sections if there are multiple BSS sections.
		u4 AddressOfEntryPoint{};
		//	The address of the entry point relative to the image base when the executable file is loaded into memory.For program images, this is the starting address.For device drivers, this is the address of the initialization function.An entry point is optional for DLLs.When no entry point is present, this field must be zero.
		u4 BaseOfCode{};
		//	The address that is relative to the image base of the beginning - of - code section when it is loaded into memory.
		u4 BaseOfData{};
		// (32 bit only) The address that is relative to the image base of the beginning-of-data section when it is loaded into memory.

		u8 ImageBase{};
		//	The preferred address of the first byte of image when loaded into memory; must be a multiple of 64�K.The default for DLLs is 0x10000000. The default for Windows CE EXEs is 0x00010000. The default for Windows�NT, Windows 2000, Windows�XP, Windows�95, Windows�98, and Windows�Me is 0x00400000.
		u4 SectionAlignment{};
		//	The alignment(in bytes) of sections when they are loaded into memory.It must be greater than or equal to FileAlignment.The default is the page size for the architecture.
		u4 FileAlignment{};
		//	The alignment factor(in bytes) that is used to align the raw data of sections in the image file.The value should be a power of 2 between 512 and 64�K, inclusive.The default is 512. If the SectionAlignment is less than the architecture�s page size, then FileAlignment must match SectionAlignment.
		u2 MajorOperatingSystemVersion{}; //	The major version number of the required operating system.
		u2 MinorOperatingSystemVersion{}; //	The minor version number of the required operating system.
		u2 MajorImageVersion{}; //	The major version number of the image.
		u2 MinorImageVersion{}; //	The minor version number of the image.
		u2 MajorSubsystemVersion{}; //	The major version number of the subsystem.
		u2 MinorSubsystemVersion{}; //	The minor version number of the subsystem.
		u4 Win32VersionValue{}; //	Reserved, must be zero.
		u4 SizeOfImage{};
		//	The size(in bytes) of the image, including all headers, as the image is loaded in memory.It must be a multiple of SectionAlignment.
		u4 SizeOfHeaders{};
		//	The combined size of an MSDOS stub, PE header, and section headers rounded up to a multiple of FileAlignment.
		u4 CheckSum{};
		//	The image file checksum.The algorithm for computing the checksum is incorporated into IMAGHELP.DLL.The following are checked for validation at load time : all drivers, any DLL loaded at boot time, and any DLL that is loaded into a critical Windows process.
		u2 Subsystem{};
		// The subsystem that is required to run this image.For more information, see �Windows Subsystem� later in this specification.
		u2 DllCharacteristics{}; //	For more information, see �DLL Characteristics� later in this specification.
		u8 SizeOfStackReserve{};
		// The size of the stack to reserve.Only SizeOfStackCommit is committed; the rest is made available one page at a time until the reserve size is reached.
		u8 SizeOfStackCommit{}; //	The size of the stack to commit.
		u8 SizeOfHeapReserve{};
		//	The size of the local heap space to reserve.Only SizeOfHeapCommit is committed; the rest is made available one page at a time until the reserve size is reached.
		u8 SizeOfHeapCommit{}; //	The size of the local heap space to commit.
		u4 LoaderFlags{}; //	Reserved, must be zero.
		u4 NumberOfRvaAndSizes{};
		//	The number of data - directory entries in the remainder of the optional header.Each describes a location and size.

		std::vector <DATA_DIRECTORY> DataDirectory{};

		enum IMAGE_SUBSYSTEM {
			IMAGE_SUBSYSTEM_UNKNOWN = 0, // An unknown subsystem
			IMAGE_SUBSYSTEM_NATIVE = 1, // Device drivers and native Windows processes
			IMAGE_SUBSYSTEM_WINDOWS_GUI = 2, // The Windows graphical user interface(GUI) subsystem
			IMAGE_SUBSYSTEM_WINDOWS_CUI = 3, // The Windows character subsystem
			IMAGE_SUBSYSTEM_POSIX_CUI = 7, // The Posix character subsystem
			IMAGE_SUBSYSTEM_WINDOWS_CE_GUI = 9, // Windows CE
			IMAGE_SUBSYSTEM_EFI_APPLICATION = 10, // An Extensible Firmware Interface(EFI) application
			IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11, // An EFI driver with boot services
			IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER = 12, // An EFI driver with run - time services
			IMAGE_SUBSYSTEM_EFI_ROM = 13, // An EFI ROM image
			IMAGE_SUBSYSTEM_XBOX = 14 // XBOX
		};

		enum IMAGE_DLLCHARACTERISTICS {
			IMAGE_DLLCHARACTERISTICS_RESERVED1 = 0x0001, //	Reserved, must be zero.
			IMAGE_DLLCHARACTERISTICS_RESERVED2 = 0x0002, //	Reserved, must be zero.
			IMAGE_DLLCHARACTERISTICS_RESERVED3 = 0x0004, //	Reserved, must be zero.
			IMAGE_DLLCHARACTERISTICS_RESERVED4 = 0x0008, //	Reserved, must be zero.
			IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA = 0x0020,
			//	Image can handle a high entropy 64 - bit virtual address space.
			IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE = 0x0040, //	DLL can be relocated at load time.
			IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY = 0x0080, //	Code Integrity checks are enforced.
			IMAGE_DLLCHARACTERISTICS_NX_COMPAT = 0x0100, //	Image is NX compatible.
			IMAGE_DLLCHARACTERISTICS_NO_ISOLATION = 0x0200, //	Isolation aware, but do not isolate the image.
			IMAGE_DLLCHARACTERISTICS_NO_SEH = 0x0400,
			//	Does not use structured exception(SE) handling.No SE handler may be called in this image.
			IMAGE_DLLCHARACTERISTICS_NO_BIND = 0x0800, //	Do not bind the image.
			IMAGE_DLLCHARACTERISTICS_APPCONTAINER = 0x1000, //	Image must execute in an AppContainer.
			IMAGE_DLLCHARACTERISTICS_WDM_DRIVER = 0x2000, //	A WDM driver.
			IMAGE_DLLCHARACTERISTICS_GUARD_CF = 0x4000, //	Image supports Control Flow Guard.
			IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE = 0x8000 //	Terminal Server aware.
		};
	};

	class SECTION_NAME {
		friend bsw::istream_wrapper& operator>>(bsw::istream_wrapper& is, SECTION_NAME& x);
		public:
			SECTION_NAME();
			explicit SECTION_NAME(u8 a);
			[[nodiscard]] std::string to_string() const;
			[[nodiscard]] u8 qword() const;

		private:
			union {
				u8 Qword;
				char Str[8];
			} name;
	};

	bool operator<(const SECTION_NAME& a, const SECTION_NAME& b);
	bool operator==(const SECTION_NAME& a, const SECTION_NAME& b);
	bool operator!=(const SECTION_NAME& a, const SECTION_NAME& b);
	std::ostream& operator<<(std::ostream& os, const SECTION_NAME& s);

	inline
	bsw::istream_wrapper& operator>>(bsw::istream_wrapper& is, SECTION_NAME& x) {
		is >> x.name.Qword;
		return is;
	}

	struct SECTION {
		SECTION_NAME Name;
		// An 8 - byte, null - padded UTF - 8 encoded string.If the string is exactly 8�characters long, there is no terminating null.For longer names, this field contains a slash(/ ) that is followed by an ASCII representation of a decimal number that is an offset into the string table.Executable images do not use a string table and do not support section names longer than 8�characters.Long names in object files are truncated if they are emitted to an executable file.
		u4 VirtualSize{};
		// The total size of the section when loaded into memory.If this value is greater than SizeOfRawData, the section is zero - padded.This field is valid only for executable images and should be set to zero for object files.
		u4 VirtualAddress{};
		// For executable images, the address of the first byte of the section relative to the image base when the section is loaded into memory.For object files, this field is the address of the first byte before relocation is applied; for simplicity, compilers should set this to zero.Otherwise, it is an arbitrary value that is subtracted from offsets during relocation.
		u4 SizeOfRawData{};
		// The size of the section(for object files) or the size of the initialized data on disk(for image files).For executable images, this must be a multiple of FileAlignment from the optional header.If this is less than VirtualSize, the remainder of the section is zero - filled.Because the SizeOfRawData field is rounded but the VirtualSize field is not, it is possible for SizeOfRawData to be greater than VirtualSize as well.When a section contains only uninitialized data, this field should be zero.
		u4 PointerToRawData{};
		//	The file pointer to the first page of the section within the COFF file.For executable images, this must be a multiple of FileAlignment from the optional header.For object files, the value should be aligned on a 4byte boundary for best performance.When a section contains only uninitialized data, this field should be zero.
		u4 PointerToRelocations{};
		//	The file pointer to the beginning of relocation entries for the section.This is set to zero for executable images or if there are no relocations.
		u4 PointerToLinenumbers{};
		//	The file pointer to the beginning of line - number entries for the section.This is set to zero if there are no COFF line numbers.This value should be zero for an image because COFF debugging information is deprecated.
		u2 NumberOfRelocations{};
		//	The number of relocation entries for the section.This is set to zero for executable images.
		u2 NumberOfLinenumbers{};
		//	The number of line - number entries for the section.This value should be zero for an image because COFF debugging information is deprecated.
		u4 Characteristics{};
		//	The flags that describe the characteristics of the section.For more information, see section 4.1, �Section Flags.�

		enum CHARACTERISITICS {
			IMAGE_SCN_TYPE_NO_PAD = 0x00000008,
			//	The section should not be padded to the next boundary.This flag is obsolete and is replaced by IMAGE_SCN_ALIGN_1BYTES.This is valid only for object files.
			IMAGE_SCN_CNT_CODE = 0x00000020, //	The section contains executable code.
			IMAGE_SCN_CNT_INITIALIZED_DATA = 0x00000040, //	The section contains initialized data.
			IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x00000080, //	The section contains uninitialized data.
			IMAGE_SCN_LNK_OTHER = 0x00000100, //	Reserved for future use.
			IMAGE_SCN_LNK_INFO = 0x00000200,
			//	The section contains comments or other information.The.drectve section has this type.This is valid for object files only.
			IMAGE_SCN_LNK_REMOVE = 0x00000800,
			//	The section will not become part of the image.This is valid only for object files.
			IMAGE_SCN_LNK_COMDAT = 0x00001000,
			//	The section contains COMDAT data.For more information, see section 5.5.6, �COMDAT Sections(Object Only).� This is valid only for object files.
			IMAGE_SCN_GPREL = 0x00008000, //	The section contains data referenced through the global pointer(GP).
			IMAGE_SCN_ALIGN_1BYTES = 0x00100000, //	Align data on a 1 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_2BYTES = 0x00200000, //	Align data on a 2 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_4BYTES = 0x00300000, //	Align data on a 4 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_8BYTES = 0x00400000, //	Align data on an 8 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_16BYTES = 0x00500000, //	Align data on a 16 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_32BYTES = 0x00600000, //	Align data on a 32 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_64BYTES = 0x00700000, //	Align data on a 64 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_128BYTES = 0x00800000, //	Align data on a 128 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_256BYTES = 0x00900000, //	Align data on a 256 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_512BYTES = 0x00A00000, //	Align data on a 512 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_1024BYTES = 0x00B00000, //	Align data on a 1024 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_2048BYTES = 0x00C00000, //	Align data on a 2048 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_4096BYTES = 0x00D00000, //	Align data on a 4096 - byte boundary.Valid only for object files.
			IMAGE_SCN_ALIGN_8192BYTES = 0x00E00000,
			//	Align data on an 8192 - byte boundary.Valid only for object files.
			IMAGE_SCN_LNK_NRELOC_OVFL = 0x01000000, //	The section contains extended relocations.
			IMAGE_SCN_MEM_DISCARDABLE = 0x02000000, //	The section can be discarded as needed.
			IMAGE_SCN_MEM_NOT_CACHED = 0x04000000, //	The section cannot be cached.
			IMAGE_SCN_MEM_NOT_PAGED = 0x08000000, //	The section is not pageable.
			IMAGE_SCN_MEM_SHARED = 0x10000000, //	The section can be shared in memory.
			IMAGE_SCN_MEM_EXECUTE = 0x20000000, //	The section can be executed as code.
			IMAGE_SCN_MEM_READ = 0x40000000, //	The section can be read.
			IMAGE_SCN_MEM_WRITE = 0x80000000 //	The section can be written to.
		};
	};

	static_assert(sizeof(SECTION) == 40, "SECTION size should be 40 bytes");

	std::ostream& operator<<(std::ostream& os, SECTION::CHARACTERISITICS v);
	std::ostream& operator<<(std::ostream& os, const SECTION& v);

	enum class DataDirectory {
		Export = 0,
		Import = 1,
		Resource = 2,
		Exception = 3,
		Certificate = 4,
		Base = 5,
		Debug = 6,
		Architecture = 7,
		Global = 8,
		TLS = 9,
		Load = 10,
		Bound = 11,
		IAT = 12,
		Delay = 13,
		CLR = 14,
		Reserved = 15
	};

	std::ostream& operator<<(std::ostream& os, OPTIONAL_HEADER::IMAGE_SUBSYSTEM v);
	std::ostream& operator<<(std::ostream& os, OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS v);
	std::ostream& operator<<(std::ostream& os, const OPTIONAL_HEADER& v);

	namespace detail {

		template<typename T>
		struct flag_traits;

#define BEGIN_FLAG_TRAITS(TYPE)							\
    template <> struct flag_traits <TYPE>				\
    {													\
    static constexpr uint32_t data[] =

#define END_FLAG_TRAITS									\
	;													\
	    static constexpr std::size_t len()				\
	    {												\
	        return sizeof(data) / sizeof(uint32_t);		\
	    }												\
    }
		// ===========================================================================
		BEGIN_FLAG_TRAITS(COFF_HEADER::IMAGE_FILE_MACHINE){
			COFF_HEADER::IMAGE_FILE_MACHINE_UNKNOWN,
			COFF_HEADER::IMAGE_FILE_MACHINE_AM33,
			COFF_HEADER::IMAGE_FILE_MACHINE_AMD64,
			COFF_HEADER::IMAGE_FILE_MACHINE_ARM,
			COFF_HEADER::IMAGE_FILE_MACHINE_ARMNT,
			COFF_HEADER::IMAGE_FILE_MACHINE_EBC,
			COFF_HEADER::IMAGE_FILE_MACHINE_I386,
			COFF_HEADER::IMAGE_FILE_MACHINE_IA64,
			COFF_HEADER::IMAGE_FILE_MACHINE_M32R,
			COFF_HEADER::IMAGE_FILE_MACHINE_MIPS16,
			COFF_HEADER::IMAGE_FILE_MACHINE_MIPSFPU,
			COFF_HEADER::IMAGE_FILE_MACHINE_MIPSFPU16,
			COFF_HEADER::IMAGE_FILE_MACHINE_POWERPC,
			COFF_HEADER::IMAGE_FILE_MACHINE_POWERPCFP,
			COFF_HEADER::IMAGE_FILE_MACHINE_R4000,
			COFF_HEADER::IMAGE_FILE_MACHINE_RISCV32,
			COFF_HEADER::IMAGE_FILE_MACHINE_RISCV64,
			COFF_HEADER::IMAGE_FILE_MACHINE_RISCV128,
			COFF_HEADER::IMAGE_FILE_MACHINE_SH3,
			COFF_HEADER::IMAGE_FILE_MACHINE_SH3DSP,
			COFF_HEADER::IMAGE_FILE_MACHINE_SH4,
			COFF_HEADER::IMAGE_FILE_MACHINE_SH5,
			COFF_HEADER::IMAGE_FILE_MACHINE_THUMB,
			COFF_HEADER::IMAGE_FILE_MACHINE_WCEMIPSV2
		}END_FLAG_TRAITS;

		BEGIN_FLAG_TRAITS(COFF_HEADER::IMAGE_FILE_CHARACTERISTICS){
			COFF_HEADER::IMAGE_FILE_RELOCS_STRIPPED,
			COFF_HEADER::IMAGE_FILE_EXECUTABLE_IMAGE,
			COFF_HEADER::IMAGE_FILE_LINE_NUMS_STRIPPED,
			COFF_HEADER::IMAGE_FILE_LOCAL_SYMS_STRIPPED,
			COFF_HEADER::IMAGE_FILE_AGGRESSIVE_WS_TRIM,
			COFF_HEADER::IMAGE_FILE_LARGE_ADDRESS_AWARE,
			COFF_HEADER::IMAGE_FILE_RESERVED,
			COFF_HEADER::IMAGE_FILE_BYTES_REVERSED_LO,
			COFF_HEADER::IMAGE_FILE_32BIT_MACHINE,
			COFF_HEADER::IMAGE_FILE_DEBUG_STRIPPED,
			COFF_HEADER::IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP,
			COFF_HEADER::IMAGE_FILE_NET_RUN_FROM_SWAP,
			COFF_HEADER::IMAGE_FILE_SYSTEM,
			COFF_HEADER::IMAGE_FILE_DLL,
			COFF_HEADER::IMAGE_FILE_UP_SYSTEM_ONLY,
			COFF_HEADER::IMAGE_FILE_BYTES_REVERSED_HI
		}END_FLAG_TRAITS;

		BEGIN_FLAG_TRAITS(OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS){
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED1,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED2,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED3,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_RESERVED4,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NX_COMPAT,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NO_ISOLATION,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NO_SEH,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_NO_BIND,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_APPCONTAINER,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_WDM_DRIVER,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_GUARD_CF,
			OPTIONAL_HEADER::IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE
		}END_FLAG_TRAITS;

		BEGIN_FLAG_TRAITS(OPTIONAL_HEADER::IMAGE_SUBSYSTEM){
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_UNKNOWN,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_NATIVE,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_WINDOWS_GUI,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_WINDOWS_CUI,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_POSIX_CUI,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_WINDOWS_CE_GUI,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_APPLICATION,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_EFI_ROM,
			OPTIONAL_HEADER::IMAGE_SUBSYSTEM_XBOX
		}END_FLAG_TRAITS;

		BEGIN_FLAG_TRAITS(SECTION::CHARACTERISITICS){
			SECTION::IMAGE_SCN_TYPE_NO_PAD,
			SECTION::IMAGE_SCN_CNT_CODE,
			SECTION::IMAGE_SCN_CNT_INITIALIZED_DATA,
			SECTION::IMAGE_SCN_CNT_UNINITIALIZED_DATA,
			SECTION::IMAGE_SCN_LNK_OTHER,
			SECTION::IMAGE_SCN_LNK_INFO,
			SECTION::IMAGE_SCN_LNK_REMOVE,
			SECTION::IMAGE_SCN_LNK_COMDAT,
			SECTION::IMAGE_SCN_GPREL,
			SECTION::IMAGE_SCN_ALIGN_1BYTES,
			SECTION::IMAGE_SCN_ALIGN_2BYTES,
			SECTION::IMAGE_SCN_ALIGN_4BYTES,
			SECTION::IMAGE_SCN_ALIGN_8BYTES,
			SECTION::IMAGE_SCN_ALIGN_16BYTES,
			SECTION::IMAGE_SCN_ALIGN_32BYTES,
			SECTION::IMAGE_SCN_ALIGN_64BYTES,
			SECTION::IMAGE_SCN_ALIGN_128BYTES,
			SECTION::IMAGE_SCN_ALIGN_256BYTES,
			SECTION::IMAGE_SCN_ALIGN_512BYTES,
			SECTION::IMAGE_SCN_ALIGN_1024BYTES,
			SECTION::IMAGE_SCN_ALIGN_2048BYTES,
			SECTION::IMAGE_SCN_ALIGN_4096BYTES,
			SECTION::IMAGE_SCN_ALIGN_8192BYTES,
			SECTION::IMAGE_SCN_LNK_NRELOC_OVFL,
			SECTION::IMAGE_SCN_MEM_DISCARDABLE,
			SECTION::IMAGE_SCN_MEM_NOT_CACHED,
			SECTION::IMAGE_SCN_MEM_NOT_PAGED,
			SECTION::IMAGE_SCN_MEM_SHARED,
			SECTION::IMAGE_SCN_MEM_EXECUTE,
			SECTION::IMAGE_SCN_MEM_READ,
			static_cast <u4>(SECTION::IMAGE_SCN_MEM_WRITE)
		}END_FLAG_TRAITS;

		template<typename T>
		bool check_in_enum(uint32_t v) {
			for (std::size_t i = 0; i < flag_traits <T>::len(); i++) {
				if (v == flag_traits <T>::data[i]) {
					return true;
				}
			}
			return false;
		}

		// ===========================================================================
		template<typename T>
		bool check_in_flags(uint32_t v) {
			for (std::size_t i = 0; i < flag_traits <T>::len(); i++) {
				const uint32_t mask = ~flag_traits <T>::data[i];
				v = v & mask;
			}
			return (v == 0);
		}
	}
}

#endif
