#ifndef PEFILE_IMPORTS_PARSER_HH_
#define PEFILE_IMPORTS_PARSER_HH_

#include <map>
#include <iosfwd>


namespace assets::pefile {
	class windows_pe_file;

	struct import_entry {
		explicit import_entry (int ord);
		explicit import_entry (std::string  nm);

		std::string name;
		int ordinal;
	};

	std::ostream& operator<< (std::ostream& os, const import_entry& x);

	using imports_table_t = std::multimap<std::string, import_entry>;
	void parse_imports (const windows_pe_file& pefile, imports_table_t& imports);
	std::size_t count_imports (const windows_pe_file& pefile);
}

#endif
