//
// Created by igor on 6/23/24.
//

#include <assets/resources/audio/music_data_manager.hh>
#include <sdlpp/io/rwops_stream.hh>

namespace neutrino::assets {

	class mus_loader : public abstract_resource_loader<sdl::music> {
		bool accept([[maybe_unused]] std::istream& is) const override {
			return true;
		};
		sdl::music load(std::istream& is) const override {
			sdl::rw_istream rwops(is);
			return sdl::music(rwops);
		}
	};

	music_data_manager::music_data_manager() {
		register_loader("music", std::make_unique<mus_loader>());
	}
}
