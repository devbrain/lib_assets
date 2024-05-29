#ifndef __PEFILE_RESOURCE_BUILDER_HPP__
#define __PEFILE_RESOURCE_BUILDER_HPP__

#include "resource_directory.hh"
#include "pefile.hh"

namespace assets::pefile {
	void build_resources (const file_c& pefile, resource_dir_c& rd);

} // ns pefile

#endif
