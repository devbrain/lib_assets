#ifndef ASSETS_SRC_ASSETS_IMAGE_BMP_HH_
#define ASSETS_SRC_ASSETS_IMAGE_BMP_HH_


#include <iosfwd>
#include "assets/resources/image/image_info.hh"
#include <assets/resources/abstract_resource_loader.hh>

namespace assets
{
	namespace detail {
		bool is_bmp (std::istream& is);
		image_info bmp_info (std::istream& is);
		neutrino::sdl::surface bmp_load (std::istream& is);
	}

	class bmp_image_resource_loader : public abstract_resource_loader<neutrino::sdl::surface, image_info> {
	 private:
		bool accept (std::istream& is) const override {
			return detail::is_bmp (is);
		}

		neutrino::sdl::surface load (std::istream& is) const override {
			return detail::bmp_load (is);
		}

		image_info info (std::istream& is) const override {
			return detail::bmp_info (is);
		}
	};
}

#endif
