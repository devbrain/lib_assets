//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILES_SET_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_TILES_SET_HH

#include <vector>
#include <assets/assets_export.h>
#include <assets/resources/world/world_component.hh>
#include <assets/resources/world/types.hh>
#include <sdlpp/video/geometry.hh>

namespace neutrino::assets {
	class ASSETS_EXPORT tiles_set : public component {
		public:
			tiles_set(image_id_t image_id,
			          unsigned firstgid,
			          const sdl::area_type& tile_dims,
			          unsigned tilecount);

			[[nodiscard]] image_id_t get_image() const;

			[[nodiscard]] unsigned get_first_gid() const;
			[[nodiscard]] sdl::area_type get_tile_dims() const;
			[[nodiscard]] unsigned size() const;

			void set_rect(local_tile_id_t tid, const sdl::rect& r);
			[[nodiscard]] sdl::rect get_rect(local_tile_id_t tid) const;

			[[nodiscard]] local_tile_id_t from_global(tile_id_t tid) const;
			[[nodiscard]] tile_id_t from_local(local_tile_id_t tid) const;

			component& get_property(local_tile_id_t tid);
			[[nodiscard]] const component& get_property(local_tile_id_t tid) const;

			[[nodiscard]] const std::vector <sdl::rect>& get_rects() const;
		private:
			image_id_t m_image_id;
			unsigned m_firstgid;
			sdl::area_type m_tile_dims;
			unsigned m_tilecount;
			std::vector <sdl::rect> m_rects;
			std::vector <component> m_props;
	};

	inline
	bool operator <(const tiles_set& a, const tiles_set& b) {
		return a.get_first_gid() < b.get_first_gid();
	}
}

#endif
