#include <assets/resources/exe/winres/resource_directory.hh>

#include "windows_pe_file.hh"
#include <bsw/strings/wchar.hh>
#include <bsw/exception.hh>
#include <bsw/strings/number_formatter.hh>

namespace neutrino::assets {
	std::string to_string(windows_resource_type resource_type) {
		switch (resource_type) {
			case CURSOR: return "CURSOR";
			case BITMAP: return "BITMAP";
			case ICON: return "ICON";
			case MENU: return "MENU";
			case DIALOG: return "DIALOG";
			case STRINGTABLE: return "STRINGTABLE";
			case FONTDIR: return "FONTDIR";
			case FONT: return "FONT";
			case ACCELERATORS: return "ACCELERATORS";
			case RCDATA: return "RCDATA";
			case MESSAGETABLE: return "MESSAGETABLE";
			case GROUP_CURSOR: return "GROUP_CURSOR";
			case GROUP_ICON: return "GROUP_ICON";
			case VERSION: return "VERSION";
			case NEWBITMAP: return "NEWBITMAP";
			case NEWMENU: return "NEWMENU";
			case NEWDIALOG: return "NEWDIALOG";
			case DLGINCLUDE: return "DLGINCLUDE";
			case PLUGPLAY: return "PLUGPLAY";
			case VXD: return "VXD";
			case ANICURSOR: return "ANICURSOR";
			case ANIICON: return "ANIICON";
			case HTML: return "HTML";
			case MANIFEST: return "MANIFEST";
			default: return bsw::number_formatter::format(resource_type);
		}
	}

	std::wstring to_wstring(windows_resource_type resource_type) {
		return bsw::utf8_to_wstring(to_string(resource_type));
	}

	// ====================================================================
	windows_resource_name::windows_resource_name(int id)
		: m_value(std::make_pair(id, L"")) {
	}

	// --------------------------------------------------------------------
	windows_resource_name::windows_resource_name(const std::wstring& name)
		: m_value(std::make_pair(-1, name)) {
	}

	// --------------------------------------------------------------------
	bool windows_resource_name::is_id() const {
		return m_value.second.empty();
	}

	// --------------------------------------------------------------------
	int windows_resource_name::id() const {
		return m_value.first;
	}

	// --------------------------------------------------------------------
	void windows_resource_name::id(int x) {
		m_value.first = x;
		m_value.second = L"";
	}

	// --------------------------------------------------------------------
	std::wstring windows_resource_name::name() const {
		return m_value.second;
	}

	// --------------------------------------------------------------------
	void windows_resource_name::name(const std::wstring& x) {
		m_value.first = -1;
		m_value.second = x;
	}

	// --------------------------------------------------------------------
	bool operator<(const windows_resource_name& a, const windows_resource_name& b) {
		return a.m_value < b.m_value;
	}

	// --------------------------------------------------------------------
	std::wostream& operator<<(std::wostream& os, const windows_resource_name& a) {
		if (a.is_id()) {
			os << a.id();
		} else {
			os << a.name();
		}
		return os;
	}

	// --------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& os, const windows_resource_name& a) {
		if (a.is_id()) {
			os << a.id();
		} else {
			os << bsw::wstring_to_utf8(a.name());
		}
		return os;
	}

	// ============================================================================
	windows_resource::windows_resource()
		: m_language_code(0),
		  m_offset(0),
		  m_size(0) {
	}

	// -------------------------------------------------------------------------
	int windows_resource::language_code() const {
		return m_language_code;
	}

	// -------------------------------------------------------------------------
	void windows_resource::language_code(int x) {
		m_language_code = x;
	}

	// -------------------------------------------------------------------------
	uint32_t windows_resource::offset() const {
		return m_offset;
	}

	// -------------------------------------------------------------------------
	void windows_resource::offset(uint32_t x) {
		m_offset = x;
	}

	// -------------------------------------------------------------------------
	uint32_t windows_resource::size() const {
		return m_size;
	}

	// -------------------------------------------------------------------------
	void windows_resource::size(uint32_t x) {
		m_size = x;
	}

	const windows_resource_name& windows_resource::name() const {
		return m_rn;
	}

	// -------------------------------------------------------------------------
	void windows_resource::name(windows_resource_name& rn) {
		m_rn = rn;
	}

	// =======================================================================
	windows_resource_directory::iterator windows_resource_directory::begin(const windows_resource_name& rn) const {
		auto i = m_dir.find(rn);
		if (i == m_dir.end()) {
			RAISE_EX("No main section found for ", rn);
		}
		return i->second.begin();
	}

	// -------------------------------------------------------------------------
	windows_resource_directory::iterator windows_resource_directory::end(const windows_resource_name& rn) const {
		auto i = m_dir.find(rn);
		if (i == m_dir.end()) {
			RAISE_EX("No main section found for ", rn);
		}
		return i->second.end();
	}

	// -----------------------------------------------------------------------
	bool windows_resource_directory::exists(int id) const {
		windows_resource_name rn(id);
		return m_dir.find(rn) != m_dir.end();
	}

	// -----------------------------------------------------------------------
	windows_resource_directory::names_iterator windows_resource_directory::names_begin() const {
		return names_iterator(m_dir.begin());
	}

	// -----------------------------------------------------------------------
	windows_resource_directory::names_iterator windows_resource_directory::names_end() const {
		return names_iterator(m_dir.end());
	}

	windows_resource_directory::windows_resource_directory() = default;

	windows_resource_directory::windows_resource_directory(windows_resource_directory&& other) noexcept = default;

	windows_resource_directory::~windows_resource_directory() = default;

	// =======================================================================
	windows_resource_directory::names_iterator& windows_resource_directory::names_iterator::operator++() {
		++m_itr;
		return *this;
	}

	// -----------------------------------------------------------------------
	bool windows_resource_directory::names_iterator::operator==(const names_iterator& a) const {
		return this->m_itr == a.m_itr;
	}

	// -----------------------------------------------------------------------
	bool windows_resource_directory::names_iterator::operator!=(const names_iterator& a) const {
		return !(*this == a);
	}

	// -----------------------------------------------------------------------
	const windows_resource_name* windows_resource_directory::names_iterator::operator->() const {
		return &m_itr->first;
	}

	// -----------------------------------------------------------------------
	const windows_resource_name& windows_resource_directory::names_iterator::operator*() const {
		return m_itr->first;
	}

	// -----------------------------------------------------------------------
	windows_resource_directory::names_iterator::names_iterator(itr_t i)
		: m_itr(i) {
	}
} // ns pefile
