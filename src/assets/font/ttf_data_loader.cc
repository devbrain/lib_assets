//
// Created by igor on 6/24/24.
//
#include <assets/resources/font/ttf_data_loader.hh>
#include <sdlpp/io/rwops_stream.hh>
namespace neutrino::assets {
	class ttf_loader : public abstract_resource_loader<sdl::ttf, point_size> {
		bool accept(std::istream& is, [[maybe_unused]] const point_size&) const override {
			return true;
		}

		sdl::ttf load(std::istream& is, const point_size& p) const override {
			sdl::rw_istream rwops(is);

			return {rwops, p.ptsize, p.hdpi, p.vdpi};
		}
	};

	ttf_data_loader::ttf_data_loader() {
		register_loader("ttf", std::make_unique<ttf_loader>());
	}
}