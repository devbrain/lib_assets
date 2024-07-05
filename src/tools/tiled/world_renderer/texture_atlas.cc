//
// Created by igor on 7/4/24.
//

#include "texture_atlas.hh"

namespace neutrino::tiled {
	void texture_atlas::clear() {
		m_textures.clear();
	}

	void texture_atlas::add(sdl::texture&& texture, std::vector<sdl::rect>&& rects) {
		m_textures.emplace_back(std::move(rects), std::move(texture));
	}

	std::tuple<const sdl::texture*, sdl::rect> texture_atlas::get(const tile& tl) const {
		if (tl.m_id.value_of() == EMPTY_TILE_VALUE) {
			return {nullptr, {}};
		}
		const auto tidx = tl.m_texture.value_of();
		const auto ridx = tl.m_id.value_of();
		return {&m_textures[tidx].m_texture, m_textures[tidx].m_rects[ridx]};
	}
}
