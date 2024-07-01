//
// Created by igor on 6/23/24.
//

#include <string>
#include <fstream>
#include <streambuf>
#include <assets/resources/world/world_data_loader.hh>
#include <sdlpp/io/rwops_stream.hh>
#include "world/tmx.hh"


namespace neutrino::assets {

	std::string path_resolver(const std::string& path) {
		std::ifstream t(path);
		return std::string((std::istreambuf_iterator<char>(t)),
						 std::istreambuf_iterator<char>());
	}

	class world_loader : public abstract_resource_loader<world> {
		bool accept([[maybe_unused]] std::istream& is) const override {
			return true;
		};
		world load(std::istream& is) const override {
			sdl::rw_istream rwops(is);
			const auto [m, a] = tmx::load(is, path_resolver);
			return std::move(m);
		}
	};

	world_data_loader::world_data_loader() {
		register_loader("TMX", std::make_unique<world_loader>());
	}
}
