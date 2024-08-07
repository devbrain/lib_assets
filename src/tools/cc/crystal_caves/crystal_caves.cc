//
// Created by igor on 7/12/24.
//
#include <fstream>
#include "crystal_caves.hh"
#include <bsw/io/binary_reader.hh>

#include "dm.hh"


std::vector<raw_map> extract_maps_cc(const std::filesystem::path& path_to_exe, const exe_map_props& props) {
	std::ifstream ifs(path_to_exe, std::ios::binary | std::ios::in);
	auto unpacked = dm.load <neutrino::assets::unpacked_exe>(ifs);

	bsw::io::memory_input_stream exe_stream(unpacked.data(), static_cast<std::streamoff>(unpacked.size()));
	exe_stream.seekg(props.get_offset(), std::ios::beg);
	bsw::io::binary_reader rdr(exe_stream);

	int level = 0;
	int rows = 0;
	std::vector<raw_map> maps;
	while (true) {
		uint8_t len;
		if (props.get_offset() != 0) {
			rdr >> len;
			if (len != props.get_columns()) {
				break;
			}
		} else {
			len = props.get_columns();
		}

		if (rows == 0) {
			maps.emplace_back(props.get_columns(), props.get_rows(level));
		}

		for (uint8_t i = 0; i < len; i++) {
			uint8_t ch;
			rdr >> ch;
			maps.back().add(ch);
		}

		rows++;
		if (rows >= props.get_rows(level)) {
			level++;
			rows = 0;
		}
	}
	return maps;
}

constexpr int INTRO = 0;
constexpr int FINALE = 1;
constexpr int MAIN = 2;

constexpr int LEVEL(int lvl) {
	return MAIN + lvl;
}


exe_map_props cc1(0x8CE0, 40, 24, {
						  {INTRO, 5},
						  {FINALE, 6},
						  {MAIN, 25},
						  {LEVEL(7), 23},
						  {LEVEL(8), 23},
						  {LEVEL(14), 23}
					  });

exe_map_props cc3(0x8F24, 40, 24, {
					  {INTRO, 5},
					  {FINALE, 6},
					  {MAIN, 25},
					  {LEVEL(7), 23},
					  {LEVEL(8), 23},
					  {LEVEL(14), 23}
				  });

using namespace neutrino;
assets::tileset load_tileset_cc(const std::filesystem::path& pth) {
	std::ifstream ifs(pth, std::ios::in | std::ios::binary);
	assets::prographx_resource rc(ifs, true, 0);
	return dm.load<assets::tileset>(rc);
}