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

	std::tuple<const sdl::texture*, sdl::rect> texture_atlas::get(const texture_id_t& tex_id) const {
		const auto tidx = tex_id.value_of();
		return {&m_textures[tidx].m_texture, m_textures[tidx].m_dims};
	}

	texture_atlas::tiles_map::tiles_map(std::vector<sdl::rect>&& rects, sdl::texture&& texture)
					: m_rects(std::move(rects)),
					  m_texture(std::move(texture)) {
		auto [px, a, w, h] = m_texture.query();
		m_dims.x = 0;
		m_dims.y = 0;
		m_dims.w = static_cast<int>(w);
		m_dims.h = static_cast<int>(h);
	}

}
