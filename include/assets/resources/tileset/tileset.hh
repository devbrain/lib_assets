//
// Created by igor on 6/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_HH

#include <map>
#include <vector>
#include <tuple>
#include <sdlpp/video/surface.hh>
#include <sdlpp/video/geometry.hh>
#include <assets/resources/image/pixels.hh>
#include <assets/resources/world/types.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	using bitmask = pixels;
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
			void set_tile(tile_id_t tid, const sdl::rect& r, bitmask&& bm);
			[[nodiscard]] sdl::rect get_tile(tile_id_t tid) const;
			[[nodiscard]] const bitmask* get_bitmap(tile_id_t tid) const;
		private:
			sdl::surface m_surface;
			tiles_map_t m_tiles_map;
			std::vector<bitmask> m_bitmaps;
	};
}

#endif
