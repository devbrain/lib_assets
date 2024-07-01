#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_STRING_TABLE_HPP_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_STRING_TABLE_HPP_

#include <map>
#include <assets/resources/exe/winres/resource_directory.hh>

namespace neutrino::assets {
	d_ASSETS_WINDOWS_RESOURCE_TRAITS(windows_rs_string_table, STRINGTABLE, false);

	class ASSETS_EXPORT windows_rs_string_table {
	 d_ASSETS_ADD_WINDOWS_RESOURCE_LOADER(windows_rs_string_table);

	 public:
		typedef std::map<int, std::wstring> strings_map_t;
	 public:
		[[nodiscard]] int number () const;

		[[nodiscard]] strings_map_t::const_iterator begin () const;
		[[nodiscard]] strings_map_t::const_iterator end () const;

	 private:
		void _number (int x);
		void _bind (int id, const std::wstring& s);
	 private:
		int m_num {};
		strings_map_t m_strings;
	};

}

#endif
