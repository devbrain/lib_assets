//
// Created by igor on 7/5/24.
//

#ifndef  ANIMATION_SEQUENCE_HH
#define  ANIMATION_SEQUENCE_HH

#include <vector>
#include <chrono>
#include <world_renderer/tile.hh>

namespace neutrino::tiled {
	class animation_sequence {
		public:
			struct frame {
				frame(const tile& tile_, const std::chrono::milliseconds& duration)
					: m_tile(tile_),
					  m_duration(duration) {
				}

				frame(const frame&) = default;
				frame& operator =(const frame&) = default;

				tile m_tile;
				std::chrono::milliseconds m_duration;
			};

			[[nodiscard]] const std::vector <frame>& get_frames() const;
			void add_frame(const frame& f);

		private:
			std::vector <frame> m_frames;
	};
}

#endif
