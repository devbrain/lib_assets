#ifndef __PEFILE_RS_STRING_TABLE_HPP__
#define __PEFILE_RS_STRING_TABLE_HPP__

#include <map>
#include "resource_directory.hh"
#include "pefile.hh"

namespace assets::pefile {
	class ASSETS_EXPORT string_table_c {
	 public:
		static constexpr int resource_id () {
			return 6;
		}

		static constexpr bool singleton () {
			return false;
		}

	 public:
		typedef std::map<int, std::wstring> strings_map_t;
	 public:
		[[nodiscard]] int number () const;

		[[nodiscard]] strings_map_t::const_iterator begin () const;
		[[nodiscard]] strings_map_t::const_iterator end () const;

		static void load (const file_c& file, const resource_c& rn, string_table_c& out);
	 private:
		void _number (int x);
		void _bind (int id, const std::wstring& s);
	 private:
		int m_num;
		strings_map_t m_strings;
	};

}

#endif
