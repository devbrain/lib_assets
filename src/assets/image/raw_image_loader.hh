//
// Created by igor on 7/9/24.
//

#ifndef  RAW_IMAGE_LOADER_HH
#define  RAW_IMAGE_LOADER_HH

#include <assets/resources/abstract_resource_loader.hh>
#include <assets/resources/image/image_data_loader.hh>
#include <sdlpp/video/surface.hh>


namespace neutrino::assets {
	class raw_image_loader : public abstract_resource_loader<sdl::surface, image_resource> {
		[[nodiscard]] bool accept (const image_resource& res) const override;
		[[nodiscard]] sdl::surface load (const image_resource& res) const override;
	};
}



#endif
