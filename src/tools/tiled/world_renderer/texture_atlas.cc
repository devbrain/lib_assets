//
// Created by igor on 7/4/24.
//

#include "texture_atlas.hh"

namespace neutrino::tiled {
	void texture_atlas::clear() {
		m_image_textures.clear();
		m_tiled_textures.clear();
	}
}
