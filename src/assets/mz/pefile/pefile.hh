#ifndef __SPY_COMMON_PEFILE_HPP__
#define __SPY_COMMON_PEFILE_HPP__

#include <string>
#include <cstdint>
#include <vector>
#include <iosfwd>
#include <memory>

#include "istream_wrapper.hh"
#include <assets/resources/exe/windows_pe.hh>
#include <assets/assets_export.h>

namespace assets::pefile {
	class abstract_reporter;

	class ASSETS_EXPORT windows_pe_file {
	 public:
		typedef std::vector<SECTION> section_t;
	 public:
		windows_pe_file (std::istream& is, abstract_reporter& reporter);

		~windows_pe_file ();
		[[nodiscard]] const COFF_HEADER& coff_header () const;
		[[nodiscard]] const OPTIONAL_HEADER& optional_header () const;
		[[nodiscard]] const section_t& sections () const;

	//	[[nodiscard]] const char* read_section (const SECTION& s) const;

		[[nodiscard]] uint32_t translate_rva (uint32_t rva) const;
		[[nodiscard]] std::istream& stream() const;
		[[nodiscard]] std::size_t file_size () const;
		[[nodiscard]] std::size_t overlay_offset () const;
	 private:
		void _load_headers (bsw::istream_wrapper& stream, abstract_reporter& reporter);
		void _check_coff_header (abstract_reporter& reporter) const;
		void _check_optional_header (abstract_reporter& reporter) const;
		void _load_sections (bsw::istream_wrapper& stream, abstract_reporter& reporter);
		[[nodiscard]] u4 _get_read_size (const SECTION& s) const;
	 private:
		void _load (abstract_reporter& reporter);
	 private:
		std::istream& m_stream;

		COFF_HEADER m_coff_header;
		OPTIONAL_HEADER m_optional_header;
		section_t m_sections;
	};


} // ns pefile

#endif
