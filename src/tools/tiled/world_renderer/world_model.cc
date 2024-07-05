//
// Created by igor on 7/5/24.
//


#include "world_model.hh"


namespace neutrino::tiled {
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

	const std::map<tile, animation_sequence>& world_model::get_animations() const {
		return m_animations;
	}

	world_model::container_t::const_iterator world_model::begin() const {
		return m_layers.begin();
	}

	world_model::container_t::const_iterator world_model::end() const {
		return m_layers.end();
	}
}
