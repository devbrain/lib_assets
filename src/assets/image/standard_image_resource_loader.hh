//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_SRC_ASSETS_IMAGE_STANDARD_IMAGE_RESOURCE_LOADER_HH_
#define ASSETS_SRC_ASSETS_IMAGE_STANDARD_IMAGE_RESOURCE_LOADER_HH_

#include <assets/resources/abstract_resource_loader.hh>
#include <assets/resources/image/image_data_loader.hh>
#include "image/stb_wrapper.hh"

namespace neutrino::assets {
	namespace detail {
		enum class stb_image_format_t {
			TGA,
			PSD
		};
		template <stb_image_format_t format>
		struct stb_image_format_traits;

		template<>
		struct stb_image_format_traits<stb_image_format_t::TGA> {
			static constexpr auto accept = &is_tga;
			static constexpr auto load = &tga_load;
		};

		template<>
		struct stb_image_format_traits<stb_image_format_t::PSD> {
			static constexpr auto accept = &is_psd;
			static constexpr auto load = &psd_load;
		};

		template <stb_image_format_t format>
		class stb_image_resource_loader : public abstract_resource_loader<sdl::surface, image_resource> {
			[[nodiscard]] bool accept (const image_resource& is) const override {
				return stb_image_format_traits<format>::accept(is.get_stream());
			}

			[[nodiscard]] sdl::surface load (const image_resource& is) const override {
				return stb_image_format_traits<format>::load(is.get_stream());
			}


		};
	}
	using tga_image_resource_loader = detail::stb_image_resource_loader<detail::stb_image_format_t::TGA>;
	using psd_image_resource_loader = detail::stb_image_resource_loader<detail::stb_image_format_t::PSD>;
}

#endif //ASSETS_SRC_ASSETS_IMAGE_STANDARD_IMAGE_RESOURCE_LOADER_HH_
