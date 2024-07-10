//
// Created by igor on 7/10/24.
//
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <assets/assets.hh>
#include <bsw/io/memory_stream_buf.hh>
#include <bsw/io/binary_reader.hh>

#include "cc_tiles_mapping.hh"

using namespace neutrino;

assets::data_manager <> dm;
constexpr int INTRO = 0;
constexpr int FINALE = 1;
constexpr int MAIN = 2;

constexpr int LEVEL(int lvl) {
	return MAIN + lvl;
}

class exe_map_props {
	public:
		exe_map_props(std::size_t offset_, int columns_, int default_rows_, const std::map <int, int>& exceptions)
			: offset(offset_), default_rows(default_rows_), columns(columns_), lvl_rows(exceptions) {
		}

		[[nodiscard]] std::streamoff get_offset() const {
			return static_cast<std::streamoff>(offset);
		}

		[[nodiscard]] int get_columns() const {
			return columns;
		}

		[[nodiscard]] int get_rows(int level) {
			auto itr = lvl_rows.find(level);
			if (itr == lvl_rows.end()) {
				return default_rows;
			}
			return itr->second;
		}

	private:
		std::size_t offset;
		int default_rows;
		int columns;
		std::map <int, int> lvl_rows;
};

std::vector<std::vector<std::string>> extract_maps(const std::filesystem::path& path_to_exe, exe_map_props& props) {
	std::ifstream ifs(path_to_exe, std::ios::binary | std::ios::in);
	auto unpacked = dm.load <assets::unpacked_exe>(ifs);

	bsw::io::memory_input_stream exe_stream(unpacked.data(), static_cast<std::streamoff>(unpacked.size()));
	exe_stream.seekg(props.get_offset(), std::ios::beg);
	bsw::io::binary_reader rdr(exe_stream);
	int level = 0;
	int rows = 0;
	std::vector<std::vector<std::string>> maps;
	while (true) {
		uint8_t len;
		rdr >> len;
		if (len != props.get_columns()) {
			break;
		}

		if (rows == 0) {
			maps.emplace_back();
		}

		std::string row;
		for (uint8_t i = 0; i < len; i++) {
			char ch;
			rdr >> ch;
			row += ch;
		}
		maps.back().emplace_back(row);
		rows++;
		if (rows >= props.get_rows(level)) {
			level++;
			rows = 0;
		}
	}
	return maps;
}

void print_maps(const std::vector<std::vector<std::string>>& maps) {
	for (const auto& lvl : maps) {
		std::cout << "------------------------------" << std::endl;
		for (const auto& row : lvl) {
			std::cout << row << std::endl;
		}
	}
}

int main(int argc, char* argv[]) {
	std::filesystem::path root = "/home/igor/proj/ares/games/CAVES";
	auto path_to_exe = root / "CC3.EXE";

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

	auto maps = extract_maps(path_to_exe, cc3);
	const auto* tlm = &tileMap;
	print_maps(maps);
	std::cout << "Maps extracted" << std::endl;
}
