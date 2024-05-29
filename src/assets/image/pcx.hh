//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_SRC_ASSETS_IMAGE_PCX_HH_
#define ASSETS_SRC_ASSETS_IMAGE_PCX_HH_

#include <iosfwd>
#include <assets/resources/image_resource_loader.hh>

namespace assets {
	namespace detail {
		bool is_pcx (std::istream& is);
		image_info pcx_info (std::istream& is);
		neutrino::sdl::surface pcx_load (std::istream& is);
	}
	class pcx_image_resource_loader : public abstract_resource_loader<neutrino::sdl::surface, image_info> {
	 private:
		bool accept (std::istream& is) const override {
			return detail::is_pcx (is);
		}

		neutrino::sdl::surface load (std::istream& is) const override {
			return detail::pcx_load (is);
		}

		image_info info (std::istream& is) const override {
			return detail::pcx_info (is);
		}
	};
}

#endif //ASSETS_SRC_ASSETS_IMAGE_PCX_HH_
