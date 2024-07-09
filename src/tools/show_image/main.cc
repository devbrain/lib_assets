//
// Created by igor on 5/29/24.
//
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>

#include <sdlpp/system.hh>
#include <sdlpp/video/window.hh>
#include <sdlpp/video/render.hh>
#include <sdlpp/events/events.hh>
#include <bsw/digest/md5.hh>
#include <assets/assets.hh>


int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "USAGE: " << argv[0] << " <path to image file" << std::endl;
		return 1;
	}

	std::filesystem::path f(argv[1]);
	auto fname = f.filename().u8string();
	std::replace (fname.begin(), fname.end(), '.', '_');


	std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
	if (!ifs) {
		std::cerr << "Can not open file " << argv[1] << std::endl;
		return 1;
	}

	try {

		using namespace neutrino;
		sdl::system initializer (sdl::init_flags::VIDEO);

		assets::image_data_loader dm;
		assets::raw_image_resource rs(ifs, assets::raw_image_resource::EGA, assets::raw_image_resource::NO_PALETTE);
		auto image = dm.load (rs);
		auto [pixels, pitch, w, h] = image.pixels_data();
		bsw::md5_engine md5;
		md5.update (image->pixels, image->pitch*image->h);
		std::cout << "static constexpr auto digest_" << fname << " = \""
		<< bsw::md5_engine::digest_to_hex(md5.digest()) << "\";" << std::endl;

		sdl::window window(w, h, sdl::window::flags::SHOWN);
		sdl::renderer renderer(window, sdl::renderer::flags::ACCELERATED);

		sdl::texture texture(renderer, image);

		bool done = false;

		while (!done) {
			// Handle Input
			neutrino::sdl::handle_input (
				[&done] (neutrino::sdl::events::quit &) {
				  done = true;
				},
				[&done] (neutrino::sdl::events::keyboard &e) {
				  if (e.pressed) {
					  done = true;
				  }
				}
			);

			renderer.clear();
			renderer.copy (texture);
			renderer.present();
		}

	} catch (std::exception& e) {
		std::cerr << "Error occured : " << e.what() << std::endl;
	}
	return 0;
}

