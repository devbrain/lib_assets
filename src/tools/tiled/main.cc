//
// Created by igor on 7/1/24.
//
#include <iostream>
#include <assets/assets.hh>
#include <sdlpp/sdlpp.hh>
#include <world_renderer/world_loader.hh>
#include <world_renderer/world_renderer.hh>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "USAGE: " << argv[0] << " <path to exe file>" << std::endl;
		return 1;
	}
	try {
		using namespace neutrino;

		assets::data_manager <> dm;

		assets::world_fs_resolver resolver(argv[1], dm.get_data_loader <neutrino::sdl::surface>());

		auto [atlas, model] = tiled::world_loader(dm.load <assets::world>(resolver));
		std::cout << "Map " << argv[1] << " loaded" << std::endl;

		sdl::system initializer(sdl::init_flags::VIDEO);
		int w = 800;
		int h = 600;

		sdl::window window(w, h, sdl::window::flags::SHOWN);
		sdl::renderer renderer(window, sdl::renderer::flags::ACCELERATED);
		int vp_w = 800;
		int vp_h = 600;
		tiled::world_renderer wr(renderer, sdl::area_type(vp_w, vp_h));
		wr.init(std::move(atlas), model);

		bool done = false;
		int camera_x = 0;
		int camera_y = 0;

		while (!done) {
			// Handle Input
			sdl::handle_input(
				[&done](sdl::events::quit&) {
					done = true;
				},
				[&done, &camera_x, &camera_y, &wr](sdl::events::keyboard& e) {
					if (e.pressed) {
						if (e.scan_code == sdl::Q || e.scan_code == sdl::ESCAPE) {
							done = true;
						}
						if (e.scan_code == sdl::UP) {
							camera_y--;
							wr.set_camera(camera_x, camera_y);
						}
						if (e.scan_code == sdl::DOWN) {
							camera_y++;
							wr.set_camera(camera_x, camera_y);
						}
						if (e.scan_code == sdl::LEFT) {
							camera_x--;
							wr.set_camera(camera_x, camera_y);
						}
						if (e.scan_code == sdl::RIGHT) {
							camera_x++;
							wr.set_camera(camera_x, camera_y);
						}
					}
				}
			);
			renderer.set_active_color(sdl::colors::black);
			renderer.clear();

			wr.update();
			sdl::rect dst_rect(10, 10, vp_w, vp_h);
			wr.present(dst_rect);
			renderer.present();

		}
	} catch (const bsw::exception& e) {
		std::cerr << e.what() << std::endl;
		e.trace().print(std::cerr, true);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
