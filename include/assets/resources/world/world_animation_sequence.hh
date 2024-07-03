//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_ANIMATION_SEQUENCE_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_ANIMATION_SEQUENCE_HH

#include <chrono>
#include <vector>
#include <assets/resources/world/types.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT animation_sequence {
		public:
			struct ASSETS_EXPORT frame {
				frame(const tile_id_t& tileid, const std::chrono::milliseconds& duration)
					: m_tileid(tileid),
					  m_duration(duration) {
				}

				tile_id_t m_tileid;
				std::chrono::milliseconds m_duration;
			};

			using frames_t = std::vector <frame>;
			animation_sequence() = default;

			void add(const tile_id_t& tileid, const std::chrono::milliseconds& duration);
			[[nodiscard]] frames_t::const_iterator begin() const;
			[[nodiscard]] frames_t::const_iterator end() const;
			[[nodiscard]] std::size_t size() const;
			[[nodiscard]] bool empty() const;
			[[nodiscard]] const frame& get(std::size_t idx) const;
		private:
			frames_t m_frames;
	};
}

#endif
