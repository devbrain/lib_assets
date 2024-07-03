//
// Created by igor on 7/3/24.
//

#include <assets/resources/world/world_tiles_set.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {
	tiles_set::tiles_set(image_id_t image_id,
	                     unsigned firstgid,
	                     const sdl::area_type& tile_dims,
	                     unsigned tilecount)
		: m_image_id(image_id),
		  m_firstgid(firstgid),
		  m_tile_dims(tile_dims),
		  m_tilecount(tilecount),
		  m_rects(tilecount),
		  m_props(tilecount) {
	}

	unsigned tiles_set::get_first_gid() const {
		return m_firstgid;
	}

	sdl::area_type tiles_set::get_tile_dims() const {
		return m_tile_dims;
	}

	unsigned tiles_set::size() const {
		return m_tilecount;
	}

	void tiles_set::set_rect(local_tile_id_t tid, const sdl::rect& r) {
		m_rects[tid.value_of()] = r;
	}

	sdl::rect tiles_set::get_rect(local_tile_id_t tid) const {
		return m_rects[tid.value_of()];
	}

	local_tile_id_t tiles_set::from_global(tile_id_t tid) const {
		ENFORCE(tid.value_of() >= m_firstgid);
		auto n = tid.value_of() - m_firstgid;
		ENFORCE(n < m_tilecount);
		return local_tile_id_t(n);
	}

	tile_id_t tiles_set::from_local(local_tile_id_t tid) const {
		ENFORCE(tid.value_of() < m_tilecount);
		return tile_id_t(m_firstgid + tid.value_of());
	}

	component& tiles_set::get_property(local_tile_id_t tid) {
		return m_props.at(tid.value_of());
	}

	const component& tiles_set::get_property(local_tile_id_t tid) const {
		return m_props.at(tid.value_of());
	}
}
