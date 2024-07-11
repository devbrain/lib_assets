//
// Created by igor on 7/10/24.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <map>
#include <assets/assets.hh>

#include <sdlpp/video/surface.hh>
#include <bsw/io/memory_stream_buf.hh>
#include <bsw/io/binary_reader.hh>

#include "raw_map.hh"
#include "cc_decode.hh"

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

std::vector<raw_map> extract_maps(const std::filesystem::path& path_to_exe, exe_map_props& props) {
	std::ifstream ifs(path_to_exe, std::ios::binary | std::ios::in);
	auto unpacked = dm.load <assets::unpacked_exe>(ifs);

	bsw::io::memory_input_stream exe_stream(unpacked.data(), static_cast<std::streamoff>(unpacked.size()));
	exe_stream.seekg(props.get_offset(), std::ios::beg);
	bsw::io::binary_reader rdr(exe_stream);
	int level = 0;
	int rows = 0;
	std::vector<raw_map> maps;
	while (true) {
		uint8_t len;
		rdr >> len;
		if (len != props.get_columns()) {
			break;
		}

		if (rows == 0) {
			maps.emplace_back(props.get_columns(), props.get_rows(level));
		}

		for (uint8_t i = 0; i < len; i++) {
			char ch;
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

void print_maps(const std::vector<raw_map>& maps) {
	for (const auto& lvl : maps) {
		std::cout << "===========================================" << std::endl;
		std::cout << lvl << std::endl;
	}
}

assets::tileset load_tileset(const std::filesystem::path& pth) {
	std::ifstream ifs(pth, std::ios::in | std::ios::binary);
	assets::prographx_resource rc(ifs, true, 0);
	return dm.load<assets::tileset>(rc);
}

void draw_layer(sdl::surface& srf, const std::vector<map_tile>& m, const assets::tileset& ts) {
	const auto& tiles = ts.get_surface();
	for (const auto& tile : m) {
		auto src_rect = ts.get_tile(assets::tile_id_t(tile.code));

		sdl::rect dst_rect(tile.posx*16, tile.posy*16, src_rect.w, src_rect.h);
		tiles.blit(src_rect, srf, dst_rect);
	}
}

sdl::surface draw_map(std::tuple <bg_map_t, fg_map_t>& mp, const assets::tileset& ts, int w, int h) {
	sdl::surface srf = sdl::surface::make_8bit(w*16, h*16);
	srf.set_palette(assets::load_standard_vga_palette());
	draw_layer(srf, std::get<0>(mp), ts);
	draw_layer(srf, std::get<1>(mp), ts);
	return srf;
}

int main(int argc, char* argv[]) {
	std::filesystem::path root = "/home/igor/proj/ares/games/CAVES";
	auto path_to_exe = root / "CC1.EXE";

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

	auto maps = extract_maps(path_to_exe, cc1);
	print_maps(maps);
	std::cout << "Maps extracted" << std::endl;

	auto ts = load_tileset(root / "CC1.GFX");
	ts.get_surface().save_bmp("ts.bmp");
	for (int i=7; i<maps.size(); i++) {
		raw_map map = maps[i];
		auto mp = cc_decode(map);
		auto srf = draw_map(mp, ts, map.get_width(), map.get_height());
		std::ostringstream os;
		os << "zopa-" << i << ".bmp";
		srf.save_bmp(os.str());
		std::cout << "Map " << i << " decoded" << std::endl;
	}
}
