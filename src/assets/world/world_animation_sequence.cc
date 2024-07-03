//
// Created by igor on 7/3/24.
//

#include <assets/resources/world/world_animation_sequence.hh>

namespace neutrino::assets {
	void animation_sequence::add(const tile_id_t& tileid, const std::chrono::milliseconds& duration) {
		m_frames.emplace_back(tileid, duration);
	}

	animation_sequence::frames_t::const_iterator animation_sequence::begin() const {
		return m_frames.begin();
	}

	animation_sequence::frames_t::const_iterator animation_sequence::end() const {
		return m_frames.end();
	}

	std::size_t animation_sequence::size() const {
		return m_frames.size();
	}

	bool animation_sequence::empty() const {
		return m_frames.empty();
	}

	const animation_sequence::frame& animation_sequence::get(std::size_t idx) const {
		return m_frames.at(idx);
	}
}
