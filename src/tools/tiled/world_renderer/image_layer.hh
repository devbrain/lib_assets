//
// Created by igor on 7/4/24.
//

#ifndef  WORLD_RENDERER_IMAGE_LAYER_HH
#define  WORLD_RENDERER_IMAGE_LAYER_HH

#include <world_renderer/types.hh>

namespace neutrino::tiled {
	class image_layer {
		public:
			explicit image_layer(texture_id_t tex_id);
			[[nodiscard]] texture_id_t get_texture_id() const;
		private:
			texture_id_t m_texture_id;
	};
}

#endif
