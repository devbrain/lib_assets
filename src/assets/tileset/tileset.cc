//
// Created by igor on 6/28/24.
//
#include <assets/resources/tileset/tileset.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {
	tileset::tileset() = default;

	tileset::tiles_map_t::const_iterator tileset::begin() const {
		return m_tiles_map.begin();
	}

	tileset::tiles_map_t::const_iterator tileset::end() {
		return m_tiles_map.end();
	}

	const sdl::surface& tileset::get_surface() const {
		return m_surface;
	}

	void tileset::set_surface(sdl::surface&& s) {
		m_surface = std::move(s);
	}

	void tileset::set_tile(tile_id_t tid, const sdl::rect& r) {
		m_tiles_map[tid] = r;
	}

	sdl::rect tileset::get_tile(tile_id_t tid) const {
		auto i = m_tiles_map.find(tid);
		if (i == m_tiles_map.end()) {
			RAISE_EX("Can not find tile ", tid);
		}
		return i->second;
	}
}
