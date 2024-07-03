//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_DETAIL_BITMAP2D_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_DETAIL_BITMAP2D_HH

#include <sul/dynamic_bitset.hpp>

namespace neutrino::assets::detail {
	class bitmap2d {
		public:
			bitmap2d()
				: m_width(0), m_height(0) {

			}

			bitmap2d(unsigned w, unsigned h)
				: m_width(w), m_height(h), m_bitmap(w * h) {
			}

			bitmap2d(const bitmap2d&) = default;
			bitmap2d& operator =(const bitmap2d&) = default;

			bitmap2d(bitmap2d&&) = default;
			bitmap2d& operator =(bitmap2d&&) = default;

			void resize(unsigned w, unsigned h) {
				m_width = w;
				m_width = h;
				m_bitmap.resize(w*h);
			}

			void set(unsigned x, unsigned y, bool v) {
				m_bitmap[m_width * y + x] = v;
			}

			[[nodiscard]] bool is_set(unsigned x, unsigned y) const {
				return m_bitmap.test(m_width * y + x);
			}

			[[nodiscard]] unsigned get_width() const {
				return m_width;
			}

			[[nodiscard]] unsigned get_height() const {
				return m_height;
			}

		private:
			unsigned m_width;
			unsigned m_height;
			sul::dynamic_bitset <> m_bitmap;
	};
}

#endif
