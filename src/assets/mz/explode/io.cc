#if defined(_MSC_VER)
#   if !defined (_CRT_SECURE_NO_WARNINGS)
#      define _CRT_SECURE_NO_WARNINGS
#    endif
#endif

#include <cstring>

#include "io.hh"
#include "bsw/exception.hh"

namespace neutrino::assets::mz::io {
	input::input () = default;
	// -------------------------------------------------------------
	input::~input () = default;

	// =============================================================
	file_input::file_input (std::istream& is)
		: m_file (is) {
	}

	// -------------------------------------------------------------
	file_input::~file_input () 	= default;

	// -------------------------------------------------------------
	void file_input::read_buff (char* buffer, std::size_t size) {
		m_file.read (buffer, size);
		if (!m_file) {
			RAISE_EX("Input error");
		}
	}

	// -------------------------------------------------------------
	offset_type file_input::tell () {
		const long pos = m_file.tellg ();
		return static_cast <offset_type> (pos);
	}

	// -------------------------------------------------------------
	offset_type file_input::bytes_remains () {
		offset_type current = tell ();
		m_file.seekg (0, std::ios::end);
		offset_type end = tell ();
		seek (current);
		return static_cast <offset_type> (end - current);
	}
	// -------------------------------------------------------------
#if defined(__SUNPRO_CC)
#if defined(_FILE_OFFSET_BITS) && (_FILE_OFFSET_BITS == 64)
#define fseek_offset fseeko64
#else 
#define fseek_offset fseeko
#endif
#else
#define fseek_offset fseek
#endif

	void file_input::seek (offset_type offset) {
		m_file.seekg (offset, std::ios::beg);

	}

	// ==============================================================
	inmem_input::inmem_input (const unsigned char* data, std::size_t size)
		: m_data (data),
		  m_size (size),
		  m_ptr (0) {
	}

	// -------------------------------------------------------------
	void inmem_input::read_buff (char* buffer, std::size_t size) {
		if (m_ptr + size > m_size) {
			RAISE_EX("Input error");
		}
		std::memcpy (buffer, m_data + m_ptr, size);
		m_ptr += size;
	}

	// -------------------------------------------------------------
	offset_type inmem_input::tell () {
		return static_cast <offset_type> (m_ptr);
	}

	// -------------------------------------------------------------
	offset_type inmem_input::bytes_remains () {
		return static_cast <offset_type> (m_size) - static_cast <offset_type> (m_ptr);
	}

	// -------------------------------------------------------------
	void inmem_input::seek (offset_type offset) {
		if (offset >= static_cast <offset_type> (m_size)) {
			RAISE_EX("Input error");
		}
		m_ptr = static_cast <std::size_t>(offset);
	}

	// =============================================================
	output::output () = default;
	// -------------------------------------------------------------
	output::~output () = default;

	// =============================================================
	file_output::file_output (const char* path)
		: m_owner (true) {
		m_file = fopen (path, "wb");
		if (!m_file) {
			RAISE_EX("Input error");
		}
	}

	// -------------------------------------------------------------
	file_output::file_output (FILE* file)
		: m_owner (false),
		  m_file (file) {
	}

	// -------------------------------------------------------------
	file_output::~file_output () {
		if (m_owner) {
			fclose (m_file);
		}
	}

	// -------------------------------------------------------------
	void file_output::write_buff (const char* buffer, std::size_t size) {
		if (fwrite (buffer, size, 1, m_file) != 1) {
			RAISE_EX("Input error");
		}
	}

	// -------------------------------------------------------------
	offset_type file_output::tell () {
		const long pos = ftell (m_file);
		if (pos <= 0) {
			RAISE_EX("Input error");
		}
		return static_cast <offset_type> (pos);
	}

	// -------------------------------------------------------------
	void file_output::seek (offset_type offset) {
		if (fseek_offset (m_file, offset, SEEK_SET) != 0) {
			RAISE_EX("Input error");
		}
	}

	// =============================================================
	inmem_output::inmem_output (std::vector<char>& out_buff)
		: m_buff (out_buff),
		  m_ptr (out_buff.size ()) {

	}

	// -------------------------------------------------------------
	void inmem_output::write_buff (const char* buffer, std::size_t size) {
		if (m_ptr + size >= m_buff.size ()) {
			m_buff.resize (m_ptr + size);
		}
		std::memcpy (&m_buff[m_ptr], buffer, size);
		m_ptr += size;
	}

	// -------------------------------------------------------------
	offset_type inmem_output::tell () {
		return static_cast <offset_type> (m_ptr);
	}

	// -------------------------------------------------------------
	void inmem_output::seek (offset_type offset) {
		if (offset >= static_cast <off_t> (m_buff.size ())) {
			RAISE_EX("Input error");
		}
		m_ptr = static_cast <std::size_t> (offset);
	}
} // ns explode
