//
// Created by igor on 6/28/24.
//
#include <limits>
#include <assets/resources/tileset/tileset.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {

	static constexpr auto INVALID_IDX = std::numeric_limits<std::size_t>::max();

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
		m_tiles_map[tid] = std::make_tuple(r, INVALID_IDX);
	}

	void tileset::set_tile(tile_id_t tid, const sdl::rect& r, const detail::bitmap2d& bm) {
		auto n = m_bitmaps.size();
		m_bitmaps.push_back(bm);
		m_tiles_map[tid] = std::make_tuple(r, n);
	}

	sdl::rect tileset::get_tile(tile_id_t tid) const {
		auto i = m_tiles_map.find(tid);
		if (i == m_tiles_map.end()) {
			RAISE_EX("Can not find tile ", tid);
		}
		return std::get<0>(i->second);
	}

	std::optional<detail::bitmap2d> tileset::get_bitmap(tile_id_t tid) const {
		auto i = m_tiles_map.find(tid);
		if (i == m_tiles_map.end()) {
			RAISE_EX("Can not find tile ", tid);
		}
		auto idx = std::get<1>(i->second);
		if (idx != INVALID_IDX) {
			return std::nullopt;
		}
		return m_bitmaps[idx];
	}
}
