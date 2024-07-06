//
// Created by igor on 7/5/24.
//

#include "world_model.hh"

namespace neutrino::tiled {
	world_model::world_model()
		: m_world_width(0),
		  m_world_height(0),
		  m_tile_width(0),
		  m_tile_height(0) {
	}

	void world_model::add_animation(const tile& base_tile, const tile& tile_,
	                                const std::chrono::milliseconds& duration) {
		animation_sequence::frame frame(tile_, duration);
		auto itr = m_animations.find(base_tile);
		if (itr == m_animations.end()) {
			animation_sequence seq;
			seq.add_frame(frame);
			m_animations.insert(std::make_pair(base_tile, seq));
		} else {
			itr->second.add_frame(frame);
		}
	}

	const std::map <tile, animation_sequence>& world_model::get_animations() const {
		return m_animations;
	}

	world_model::container_t::const_iterator world_model::begin() const {
		return m_layers.begin();
	}

	world_model::container_t::const_iterator world_model::end() const {
		return m_layers.end();
	}

	void world_model::set_geometry(unsigned tile_width, unsigned tile_height, unsigned num_tiles_x,
	                               unsigned num_tiles_y) {
		m_tile_width = world_coords_t(tile_width);
		m_tile_height = world_coords_t(tile_height);
		m_world_width = world_coords_t(tile_width * num_tiles_x);
		m_world_height = world_coords_t(tile_height * num_tiles_y);
	}

	world_coords_t world_model::get_world_width() const {
		return m_world_width;
	}

	world_coords_t world_model::get_world_height() const {
		return m_world_height;
	}

	world_coords_t world_model::get_tile_width() const {
		return m_tile_width;
	}

	world_coords_t world_model::get_tile_height() const {
		return m_tile_height;
	}
}
