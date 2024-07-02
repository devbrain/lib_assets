//
// Created by igor on 7/2/24.
//

#ifndef SSETS_INCLUDE_ASSETS_RESOURCES_WORLD_IMAGE_LAYER_HH
#define SSETS_INCLUDE_ASSETS_RESOURCES_WORLD_IMAGE_LAYER_HH

#include <assets/resources/world/world_component.hh>
#include <assets/resources/world/types.hh>
#include <sdlpp/video/geometry.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT image_layer : public component {
		public:
			image_layer(const sdl::point& offset, image_id_t image_id);

			[[nodiscard]] sdl::point get_offset() const;
			[[nodiscard]] image_id_t get_image_id() const;
		private:
			sdl::point m_offset;
			image_id_t m_image_id;
	};
}

#endif
