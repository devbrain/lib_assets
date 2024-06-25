#ifndef PEFILE_ABSTRACT_REPORTER_HH_
#define PEFILE_ABSTRACT_REPORTER_HH_

#include <cstdint>
#include <iosfwd>

namespace assets::pefile {
	class abstract_reporter {
	 public:
		enum header_field_t {
			COFF_HEADER_MACHINE_TYPE,
			COFF_HEADER_CHARACTERISTICS,

			OPTIONAL_HEADER_MAGIC,
			OPTIONAL_HEADER_IMAGE_BASE,
			OPTIONAL_HEADER_SUBSYSTEM,
			OPTIONAL_HEADER_DLL_CHARACTERISITCS,
			OPTIONAL_HEADER_SECTION_ALIGNMENT,
			OPTIONAL_HEADER_FILE_ALIGNMENT,
			OPTIONAL_HEADER_SIZE_OF_IMAGE
		};
		enum struct_name_t {
			OPTIONAL_HEADER
		};

		enum reserved_field_t {
			OPTIONAL_HEADER_WIN32VERSIONVALUE,
			OPTIONAL_HEADER_LOADER_FLAGS
		};
	 public:
		virtual ~abstract_reporter ();

		virtual void invalid_enum_value (header_field_t f, uint64_t actual_value);
		virtual void invalid_field_value (header_field_t f, uint64_t actual_value);
		virtual void invalid_struct_length (struct_name_t name, uint64_t expected, uint64_t actual);
		virtual void unknown_flag_set (header_field_t f, uint64_t actual_value);
		virtual void should_be_zero (reserved_field_t f, uint64_t actual_value);
		virtual void bad_checksum ();
	};

	std::ostream& operator<< (std::ostream& os, abstract_reporter::header_field_t f);
	std::ostream& operator<< (std::ostream& os, abstract_reporter::struct_name_t f);
	std::ostream& operator<< (std::ostream& os, abstract_reporter::reserved_field_t f);
} // ns pefile

#endif
