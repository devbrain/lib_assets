//
// Created by igor on 5/29/24.
//

#include <sdlpp/io.hh>
#include <SDL_image.h>
#include <assets/resources/image/image_data_manager.hh>
#include "image/standard_image_resource_loader.hh"

#include <bsw/macros.hh>

namespace neutrino::assets {
	namespace detail {
		enum std_image_format_t : int {
			BMP,
			ICO,
			CUR,
			GIF,
			JPG,
			LBM,
			PCX,
			PNG,
			PNM,
			SVG,
			QOI,
			XPM,
			XV
		};

		template <std_image_format_t format>
		class standard_image_resource_loader;

#define DECLARE_STD_IMG_LOADER(T)																							\
		template <>																											\
		class standard_image_resource_loader<T> : public abstract_resource_loader<neutrino::sdl::surface, image_info> { 	\
         public:                                      																		\
         	static constexpr auto NAME = STRINGIZE(T);                                             							\
		 private:																											\
			bool accept (std::istream& is) const override {      															\
				using namespace neutrino::sdl;                      														\
				rw_istream rw(is);																							\
				return PPCAT(IMG_is, T) (rw.handle());																		\
			}																												\
																															\
			neutrino::sdl::surface load (std::istream& is) const override {													\
				using namespace neutrino::sdl;																				\
				rw_istream rw(is);																							\
				SDL_Surface* srf = PPCAT(PPCAT(IMG_Load, T), _RW) (rw.handle());											\
				if (!srf) {																									\
					RAISE_EX("Failed to load [", STRINGIZE(T), "] image format");											\
				}																											\
				return surface(object<SDL_Surface>(srf, true));																\
			}																												\
																															\
			image_info info ([[maybe_unused]] std::istream& is) const override {											\
				return {NAME};																								\
			}																												\
		}

		DECLARE_STD_IMG_LOADER(BMP);
		DECLARE_STD_IMG_LOADER(ICO);
		DECLARE_STD_IMG_LOADER(CUR);
		DECLARE_STD_IMG_LOADER(GIF);
		DECLARE_STD_IMG_LOADER(JPG);
		DECLARE_STD_IMG_LOADER(LBM);
		DECLARE_STD_IMG_LOADER(PCX);
		DECLARE_STD_IMG_LOADER(PNG);
		DECLARE_STD_IMG_LOADER(PNM);
		DECLARE_STD_IMG_LOADER(SVG);
		DECLARE_STD_IMG_LOADER(QOI);
		DECLARE_STD_IMG_LOADER(XPM);
		DECLARE_STD_IMG_LOADER(XV);
	}

#define REGISTER_STD_IMG_LOADER(T) 																	\
	register_loader (detail::standard_image_resource_loader<detail::std_image_format_t::T>::NAME, 	\
		std::make_unique<detail::standard_image_resource_loader<detail::std_image_format_t::T>>())

	image_data_manager::image_data_manager () {
		
		REGISTER_STD_IMG_LOADER(BMP);
		REGISTER_STD_IMG_LOADER(ICO);
		REGISTER_STD_IMG_LOADER(CUR);
		REGISTER_STD_IMG_LOADER(GIF);
		REGISTER_STD_IMG_LOADER(JPG);
		REGISTER_STD_IMG_LOADER(LBM);
		REGISTER_STD_IMG_LOADER(PCX);
		REGISTER_STD_IMG_LOADER(PNG);
		REGISTER_STD_IMG_LOADER(PNM);
		REGISTER_STD_IMG_LOADER(SVG);
		REGISTER_STD_IMG_LOADER(QOI);
		REGISTER_STD_IMG_LOADER(XPM);
		REGISTER_STD_IMG_LOADER(XV);
		register_loader ("PSD", std::make_unique<psd_image_resource_loader>());
		register_loader ("TGA", std::make_unique<tga_image_resource_loader>());
	}
}
