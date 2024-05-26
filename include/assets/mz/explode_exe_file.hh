//
// Created by igor on 14/02/2021.
//

#ifndef TOMBEXCAVATOR_EXPLODE_EXE_FILE_HH
#define TOMBEXCAVATOR_EXPLODE_EXE_FILE_HH

#include <vector>
#include <memory>
#include <iosfwd>

#include <bsw/io/binary_reader.hh>
#include <assets/assets_export.h>


namespace assets::mz
{
    ASSETS_EXPORT bool explode_exe_file(const char* input, std::size_t input_size, std::vector<char>& output);
	ASSETS_EXPORT std::unique_ptr<bsw::io::binary_reader> explode_exe_file(std::istream& istream);
}

#endif //TOMBEXCAVATOR_EXPLODE_EXE_FILE_HH
