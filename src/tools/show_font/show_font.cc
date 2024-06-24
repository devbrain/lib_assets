//
// Created by igor on 6/24/24.
//
#include <iostream>
#include <fstream>
#include <sdlpp/sdlpp.hh>
#include <assets/resources/font/bgi_font_data_manager.hh>

static void render_glyph(const neutrino::assets::bgi_font& fnt, neutrino::sdl::renderer& r, int x, int y, char ch) {
	const auto& gd = fnt.glyphs.find(ch)->second;

	neutrino::sdl::point pen(0, 0);
	neutrino::sdl::point start(x, y);

	for (const auto& c : gd.stroke_commands) {
		if (c.move) {
			pen = c.coords;
		} else {
			r.set_active_color(neutrino::sdl::colors::red);
			r.draw_line(start + pen, start + c.coords);
			pen = c.coords;
		}
	}
}

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cerr << "USAGE: " << argv[0] << " <path to image file" << std::endl;
		return 1;
	}

	std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
	if (!ifs) {
		std::cerr << "Can not open file " << argv[1] << std::endl;
		return 1;
	}

	using namespace neutrino;
	assets::bgi_font_data_manager dm;
	auto bgi = dm.load(ifs);

	try {

		using namespace neutrino;
		sdl::system initializer (sdl::init_flags::VIDEO);
		int w = 640;
		int h = 480;

		sdl::window window(w, h, sdl::window::flags::SHOWN);
		sdl::renderer renderer(window, sdl::renderer::flags::ACCELERATED);



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
			renderer.set_active_color(neutrino::sdl::colors::black);
			renderer.clear();
			render_glyph(bgi, renderer, w/2, h/2, 'T');
			renderer.present();
		}

	} catch (std::exception& e) {
		std::cerr << "Error occured : " << e.what() << std::endl;
	}

	return 0;
}