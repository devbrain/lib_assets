#include <iostream>
#include <sstream>
#include <stdexcept>

#include "pefile.hh"
#include <assets/resources/exe/winres/resource_directory.hh>
#include "bsw/strings/wchar.hh"

namespace assets::pefile {

	// ====================================================================
	resource_name::resource_name (int id)
		: m_value (std::make_pair (id, L"")) {

	}

	// --------------------------------------------------------------------
	resource_name::resource_name (const std::wstring& name)
		: m_value (std::make_pair (-1, name)) {

	}

	// --------------------------------------------------------------------
	bool resource_name::is_id () const {
		return m_value.second.empty ();
	}

	// --------------------------------------------------------------------
	int resource_name::id () const {
		return m_value.first;
	}

	// --------------------------------------------------------------------
	void resource_name::id (int x) {
		m_value.first = x;
		m_value.second = L"";
	}

	// --------------------------------------------------------------------
	std::wstring resource_name::name () const {
		return m_value.second;
	}

	// --------------------------------------------------------------------
	void resource_name::name (const std::wstring& x) {
		m_value.first = -1;
		m_value.second = x;
	}

	// --------------------------------------------------------------------
	bool resource_name::is_special () const {
		if (is_id ()) {
			if (m_value.first >= CURSOR && m_value.first <= MANIFEST && m_value.first != 18) {
				return true;
			}
			switch (m_value.first) {
				case NEWBITMAP:
				case NEWMENU:
				case NEWDIALOG: return true;
				default: return false;
			}
		}
		return false;
	}

	// --------------------------------------------------------------------
	bool operator< (const resource_name& a, const resource_name& b) {
		return a.m_value < b.m_value;
	}

	// --------------------------------------------------------------------
	std::wostream& operator<< (std::wostream& os, const resource_name& a) {
		if (a.is_id ()) {
			os << a.id ();
		} else {
			os << a.name ();
		}
		return os;
	}

	// --------------------------------------------------------------------
	std::ostream& operator<< (std::ostream& os, const resource_name& a) {
		if (a.is_id ()) {
			os << a.id ();
		} else {
			os << bsw::wstring_to_utf8 (a.name ());
		}
		return os;
	}

	// ============================================================================
	resource::resource ()
		: m_language_code (0),
		  m_offset (0),
		  m_size (0) {

	}

	// -------------------------------------------------------------------------
	int resource::language_code () const {
		return m_language_code;
	}

	// -------------------------------------------------------------------------
	void resource::language_code (int x) {
		m_language_code = x;
	}

	// -------------------------------------------------------------------------
	uint32_t resource::offset () const {
		return m_offset;
	}

	// -------------------------------------------------------------------------
	void resource::offset (uint32_t x) {
		m_offset = x;
	}

	// -------------------------------------------------------------------------
	uint32_t resource::size () const {
		return m_size;
	}

	// -------------------------------------------------------------------------
	void resource::size (uint32_t x) {
		m_size = x;
	}

	const resource_name& resource::name () const {
		return m_rn;
	}

	// -------------------------------------------------------------------------
	void resource::name (resource_name& rn) {
		m_rn = rn;
	}

	// -------------------------------------------------------------------------
	std::size_t resource::offset_in_file (const windows_pe_file& file) const {
		const std::size_t file_size = file.file_size ();
		const auto& entry = file.optional_header ().DataDirectory[static_cast <int>(DataDirectory::Resource)];
		const uint32_t rc_offs = file.translate_rva (m_offset);

		uint64_t total = rc_offs;
		total += m_size;

		if (total >= file_size) {
			return file_size;
		}
		return rc_offs;
	}

	// =======================================================================
	resource_dir::iterator resource_dir::begin (const resource_name& rn) const {
		auto i = m_dir.find (rn);
		if (i == m_dir.end ()) {
			std::ostringstream os;
			os << "No main section found for " << rn;
			throw std::runtime_error (os.str ());
		}
		return i->second.begin ();
	}

	// -------------------------------------------------------------------------
	resource_dir::iterator resource_dir::end (const resource_name& rn) const {
		auto i = m_dir.find (rn);
		if (i == m_dir.end ()) {
			std::ostringstream os;
			os << "No main section found for " << rn;
			throw std::runtime_error (os.str ());
		}
		return i->second.end ();
	}

	// -----------------------------------------------------------------------
	bool resource_dir::exists (int id) const {
		resource_name rn (id);
		return m_dir.find (rn) != m_dir.end ();
	}

	// -----------------------------------------------------------------------
	resource_dir::names_iterator resource_dir::names_begin () const {
		return names_iterator (m_dir.begin ());
	}

	// -----------------------------------------------------------------------
	resource_dir::names_iterator resource_dir::names_end () const {
		return names_iterator (m_dir.end ());
	}

	// =======================================================================
	resource_dir::names_iterator& resource_dir::names_iterator::operator++ () {
		m_itr++;
		return *this;
	}

	// -----------------------------------------------------------------------
	bool resource_dir::names_iterator::operator== (const names_iterator& a) {
		return this->m_itr == a.m_itr;
	}

	// -----------------------------------------------------------------------
	bool resource_dir::names_iterator::operator!= (const names_iterator& a) {
		return !(*this == a);
	}

	// -----------------------------------------------------------------------
	const resource_name* resource_dir::names_iterator::operator-> () const {
		return &m_itr->first;
	}

	// -----------------------------------------------------------------------
	const resource_name& resource_dir::names_iterator::operator* () const {
		return m_itr->first;
	}

	// -----------------------------------------------------------------------
	resource_dir::names_iterator::names_iterator (itr_t i)
		: m_itr (i) {

	}
} // ns pefile
