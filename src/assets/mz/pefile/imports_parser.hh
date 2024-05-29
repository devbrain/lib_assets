#ifndef PEFILE_IMPORTS_PARSER_HH_
#define PEFILE_IMPORTS_PARSER_HH_

#include <map>
#include <iosfwd>
#include "pefile.hh"

namespace assets::pefile {
	struct import_entry_s {
		explicit import_entry_s (int ord);
		explicit import_entry_s (std::string  nm);

		std::string name;
		int ordinal;
	};

	std::ostream& operator<< (std::ostream& os, const import_entry_s& x);

	using imports_table_t = std::multimap<std::string, import_entry_s>;
	void parse_imports (const file_c& pefile, imports_table_t& imports);
	std::size_t count_imports (const file_c& pefile);
}

#endif
