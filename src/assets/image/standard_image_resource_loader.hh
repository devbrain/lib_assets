//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_SRC_ASSETS_IMAGE_STANDARD_IMAGE_RESOURCE_LOADER_HH_
#define ASSETS_SRC_ASSETS_IMAGE_STANDARD_IMAGE_RESOURCE_LOADER_HH_

#include <assets/resources/image/image_info.hh>
#include <assets/resources/abstract_resource_loader.hh>
#include "image/stb_wrapper.hh"

namespace neutrino::assets {
	namespace detail {
		enum class stb_image_format_t {
			TGA,
			PSD
		};
		template <stb_image_format_t format>
		struct stb_image_format_traits;

//		template<>
//		struct standard_image_format_traits<standard_image_format_t::GIF> {
//			static constexpr auto accept = &is_gif;
//			static constexpr auto info = &gif_info;
//			static constexpr auto load = &gif_load;
//		};
//
		template<>
		struct stb_image_format_traits<stb_image_format_t::TGA> {
			static constexpr auto accept = &is_tga;
			static constexpr auto info = &tga_info;
			static constexpr auto load = &tga_load;
		};
//
//		template<>
//		struct standard_image_format_traits<standard_image_format_t::PNG> {
//			static constexpr auto accept = &is_png;
//			static constexpr auto info = &png_info;
//			static constexpr auto load = &png_load;
//		};
//
//		template<>
//		struct standard_image_format_traits<standard_image_format_t::JPEG> {
//			static constexpr auto accept = &is_jpeg;
//			static constexpr auto info = &jpeg_info;
//			static constexpr auto load = &jpeg_load;
//		};

		template<>
		struct stb_image_format_traits<stb_image_format_t::PSD> {
			static constexpr auto accept = &is_psd;
			static constexpr auto info = &psd_info;
			static constexpr auto load = &psd_load;
		};

		template <stb_image_format_t format>
		class stb_image_resource_loader : public abstract_resource_loader<neutrino::sdl::surface, image_info> {
		 private:
			bool accept (std::istream& is) const override {
				return stb_image_format_traits<format>::accept(is);
			}

			neutrino::sdl::surface load (std::istream& is) const override {
				return stb_image_format_traits<format>::load(is);
			}

			image_info info (std::istream& is) const override {
				return stb_image_format_traits<format>::info(is);
			}
		};
	}
//	using gif_image_resource_loader = detail::standard_image_resource_loader<detail::standard_image_format_t::GIF>;
//	using png_image_resource_loader = detail::standard_image_resource_loader<detail::standard_image_format_t::PNG>;
	using tga_image_resource_loader = detail::stb_image_resource_loader<detail::stb_image_format_t::TGA>;
//	using jpeg_image_resource_loader = detail::standard_image_resource_loader<detail::standard_image_format_t::JPEG>;
	using psd_image_resource_loader = detail::stb_image_resource_loader<detail::stb_image_format_t::PSD>;
}

#endif //ASSETS_SRC_ASSETS_IMAGE_STANDARD_IMAGE_RESOURCE_LOADER_HH_
