//
// Created by igor on 7/2/24.
//

#include <assets/resources/world/world_image_layer.hh>

namespace neutrino::assets {
	image_layer::image_layer(const sdl::point& offset, image_id_t image_id)
		: m_offset(offset), m_image_id(image_id) {
	}

	sdl::point image_layer::get_offset() const {
		return m_offset;
	}

	image_id_t image_layer::get_image_id() const {
		return m_image_id;
	}
}
