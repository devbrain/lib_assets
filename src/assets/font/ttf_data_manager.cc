//
// Created by igor on 6/24/24.
//
#include <assets/resources/font/ttf_data_manager.hh>
#include <sdlpp/io/rwops_stream.hh>
namespace neutrino::assets {
	class ttf_loader : public abstract_resource_loader<sdl::ttf> {
		bool accept(std::istream& is) const override {
			return true;
		}

		sdl::ttf load(std::istream& is) const override {
			sdl::rw_istream rwops(is);
			return {}; // TODO
		}
	};

	ttf_data_manager::ttf_data_manager() {
		register_loader("ttf", std::make_unique<ttf_loader>());
	}
}