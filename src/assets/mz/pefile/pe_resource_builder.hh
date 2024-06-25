#ifndef __PEFILE_RESOURCE_BUILDER_HPP__
#define __PEFILE_RESOURCE_BUILDER_HPP__

#include <assets/resources/exe/winres/resource_directory.hh>


namespace assets::pefile {
	void build_resources (const windows_pe_file& pefile, resource_dir& rd);
} // ns pefile

#endif
