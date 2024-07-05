//
// Created by igor on 7/4/24.
//

#ifndef  WORLD_RENDERER_HH
#define  WORLD_RENDERER_HH

#include <sdlpp/video/render.hh>
#include <sdlpp/video/geometry.hh>
#include <world_renderer/world_model.hh>
#include <world_renderer/atlas_builder.hh>
#include <world_renderer/texture_atlas.hh>

namespace neutrino::tiled {
	class world_renderer {
		public:
			world_renderer(sdl::renderer& renderer,
			               sdl::area_type& dimensions,
			               sdl::pixel_format pixel_format = sdl::pixel_format(sdl::pixel_format::RGBA8888));

			void init(atlas_builder&& atlas_builder_, const world_model& model);
			void deinit();

			void update();

		private:
			void _update();
			void draw_layer (const image_layer& layer);
			void draw_layer (const color_layer& layer) const;
			void draw_layer (const tiles_layer& layer, std::chrono::milliseconds delta_time);

			sdl::renderer& m_renderer;
			sdl::area_type m_dimension;
			sdl::texture m_target;
			texture_atlas m_texture_atlas;
			const world_model* m_model;

			struct animation_data {
				explicit animation_data(const animation_sequence& seq);

				std::size_t m_current_frame;
				std::chrono::milliseconds m_time_in_current_frame;
				const animation_sequence& m_sequence;
			};

			std::map <tile, animation_data> m_animations;
			std::chrono::milliseconds m_last_update_time;
	};
}

#endif
