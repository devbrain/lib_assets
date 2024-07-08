//
// Created by igor on 7/7/24.
//

#include <assets/resources/image/pixels.hh>

namespace neutrino::assets {
	pixels::pixels(std::size_t w, std::size_t h)
		: m_width(w), m_height(h), m_pixels(w * h, 0) {
	}

	std::size_t pixels::get_width() const {
		return m_width;
	}

	std::size_t pixels::get_height() const {
		return m_height;
	}

	const uint8_t* pixels::data() const {
		return m_pixels.data();
	}

	uint8_t* pixels::data() {
		return m_pixels.data();
	}
}
