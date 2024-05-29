#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "image/icons/pe_image.hpp"
#include "mz/pefile/istream_wrapper.hh"
#include "mz/pefile/exefile.hh"

namespace spy
{
    namespace am
    {
		static void load(assets::pefile::exe_file_c& exe, icons_container_c& container)
		{
			
			std::vector<assets::pefile::icon_group_c> group;
			exe.load_resource(group);
			for (const auto& g : group)
			{
				for (auto j : g)
				{
					container.add_group(j.first, j.second);
				}
			}

			std::vector<assets::pefile::icon_info_s> icons;
			exe.load_resource(icons);
			for (const auto& ii : icons)
			{
				container.add_ordinal(ii.ordinal, ii.offset, ii.size);
			}
		}

        pe_image_c::pe_image_c (const std::string& path, icons_container_c& container)
        {
			assets::pefile::exe_file_c exe(path);
			load(exe, container);
        }
		// ----------------------------------------------------------
		pe_image_c::pe_image_c(const std::wstring& path, icons_container_c& container)
		{
			assets::pefile::exe_file_c exe(path);
			load(exe, container);
		}
        // ----------------------------------------------------------
		pe_image_c::pe_image_c(const char* data, std::size_t size, icons_container_c& container)
		{
			assets::pefile::exe_file_c exe(data, size);
			load(exe, container);
		}
    } // ns am
} // ns spy
