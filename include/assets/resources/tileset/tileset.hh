//
// Created by igor on 6/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_HH

#include <map>
#include <vector>
#include <optional>
#include <tuple>
#include <sdlpp/video/surface.hh>
#include <sdlpp/video/geometry.hh>
#include <assets/resources/detail/bitmap2d.hh>
#include <assets/resources/world/types.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT tileset {
		public:
			using tiles_map_t = std::map <tile_id_t, std::tuple<sdl::rect, std::size_t>>;

		public:
			tileset();

			[[nodiscard]] tiles_map_t::const_iterator begin() const;
			[[nodiscard]] tiles_map_t::const_iterator end();

			[[nodiscard]] const sdl::surface& get_surface() const;
			void set_surface(sdl::surface&& s);

			void set_tile(tile_id_t tid, const sdl::rect& r);
			void set_tile(tile_id_t tid, const sdl::rect& r, const detail::bitmap2d& bm);
			[[nodiscard]] sdl::rect get_tile(tile_id_t tid) const;
			[[nodiscard]] std::optional<detail::bitmap2d> get_bitmap(tile_id_t tid) const;


		private:
			sdl::surface m_surface;
			tiles_map_t m_tiles_map;
			std::vector<detail::bitmap2d> m_bitmaps;
	};
}

#endif
