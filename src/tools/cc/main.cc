//
// Created by igor on 7/10/24.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <assets/assets.hh>
#include "raw_map.hh"
#include "crystal_caves/cc_decode.hh"
#include "crystal_caves/crystal_caves.hh"
#include "secret_agent/secret_agent.hh"
#include "draw_map.hh"

using namespace neutrino;





void load_cc() {
	std::filesystem::path root = "/home/igor/proj/ares/games/CAVES";
	auto path_to_exe = root / "CC1.EXE";



	auto maps = extract_maps_cc(path_to_exe, cc1);
	std::cout << "Maps extracted" << std::endl;

	auto ts = load_tileset_cc(root / "CC1.GFX");
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



int main(int argc, char* argv[]) {
	// std::filesystem::path root = "/home/igor/proj/ares/games/Sagent";
	// auto path_to_exe = root / "SAM103.GFX";
	//
	// auto sa_maps = extract_sa_maps(path_to_exe);
	// auto ts = extract_sa_mini_tileset(root / "SAM102.GFX");
	// ts.get_surface().save_bmp("zopa_sa_mini.bmp");
	load_cc();
	return 0;

}
