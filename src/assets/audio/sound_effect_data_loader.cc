//
// Created by igor on 6/23/24.
//

#include <assets/resources/audio/sound_effect_data_loader.hh>
#include <sdlpp/io/rwops_stream.hh>

namespace neutrino::assets {
	class sound_loader : public abstract_resource_loader <sdl::sound_effect> {
		bool accept([[maybe_unused]] std::istream& is) const override {
			return true;
		};

		sdl::sound_effect load(std::istream& is) const override {
			sdl::rw_istream rwops(is);
			return sdl::sound_effect(rwops);
		}
	};

	sound_effect_data_loader::sound_effect_data_loader() {
		register_loader("sound", std::make_unique <sound_loader>());
	}
}
