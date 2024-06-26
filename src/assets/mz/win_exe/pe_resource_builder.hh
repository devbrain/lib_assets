#ifndef __PEFILE_RESOURCE_BUILDER_HPP__
#define __PEFILE_RESOURCE_BUILDER_HPP__

#include <assets/resources/exe/winres/resource_directory.hh>

namespace neutrino::assets {
	class windows_ne_file;
	class windows_pe_file;

	namespace detail {
		class resource_dir_builder {
			public:
				explicit resource_dir_builder(windows_resource_directory& rd);

				void start_main_entry(const windows_resource_name& rcname);
				void end_main_entry();

				void start_sub_entry(const windows_resource_name& rcname);
				void add_to_sub_entry(windows_resource& res);
				void end_sub_entry();

			private:
				using first_level_t = windows_resource_directory::first_level_t;
				using second_level_t = windows_resource_directory::second_level_t;

			private:
				first_level_t& m_dir;
				second_level_t m_curr;
				windows_resource_name m_curr_main_entry;
				windows_resource_name m_curr_sub_entry;
		};


	}

	void build_resources(const windows_pe_file& pefile, windows_resource_directory& rd);
	void build_resources(const windows_ne_file& pefile, windows_resource_directory& rd);
} // ns pefile

#endif
