//
// Created by igor on 6/24/24.
//
#include <assets/resources/font/ttf_data_loader.hh>
#include <sdlpp/io/rwops_stream.hh>
namespace neutrino::assets {
	class ttf_loader : public abstract_resource_loader<sdl::ttf, ttf_resource> {
		[[nodiscard]] bool accept([[maybe_unused]] const ttf_resource&) const override {
			return true;
		}

		[[nodiscard]] sdl::ttf load(const ttf_resource& p) const override {
			sdl::rw_istream rwops(p.get_stream());

			return {rwops, p.ptsize, p.hdpi, p.vdpi};
		}
	};

	std::istream& ttf_resource::get_stream() const {
		return m_stream;
	}

	ttf_data_loader::ttf_data_loader() {
		register_loader("ttf", std::make_unique<ttf_loader>());
	}
}