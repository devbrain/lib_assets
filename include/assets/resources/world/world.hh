//
// Created by igor on 06/07/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_WORLD_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_WORLD_HH

#include <map>

#include <assets/resources/world/types.hh>
#include <assets/resources/world/world_tiles_layer.hh>
#include <assets/resources/world/world_tiles_set.hh>
#include <assets/resources/world/world_animation_sequence.hh>
#include <assets/resources/world/world_object_layer.hh>
#include <assets/resources/world/world_image_layer.hh>
#include <sdlpp/video/color.hh>
#include <sdlpp/video/surface.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class world_builder;

	class ASSETS_EXPORT world : public component {
		public:
			using layer_t = std::variant <image_layer, object_layer, tiles_layer>;

		public:
			world(orientation_t orientation,
			      render_order_t render_order,
			      const sdl::area_type& map_dims,
			      const sdl::area_type& tile_dims,
			      const sdl::color& bgcolor
			);

			world(orientation_t orientation,
			      render_order_t render_order,
			      const sdl::area_type& map_dims,
			      const sdl::area_type& tile_dims,
			      const sdl::color& bgcolor,
			      unsigned int hex_side_length,
			      stagger_axis_t axis,
			      stagger_index_t index,
			      bool infinite);

			world(const world&) = delete;
			world& operator =(const world&) = delete;

			world(world&&) = default;

			template<class Functor>
			void visit_layers(Functor&& visitor) const;

			void set_empty_tile_id(tile_id_t e);
			[[nodiscard]] tile_id_t get_empty_tile_id() const;

			[[nodiscard]] orientation_t get_orientation() const;
			[[nodiscard]] render_order_t get_render_order() const;

			[[nodiscard]] sdl::area_type get_map_dimension() const;
			[[nodiscard]] sdl::area_type get_tiles_dimension() const;

			[[nodiscard]] const sdl::color& get_background_color() const;

			[[nodiscard]] unsigned int get_hex_side_length() const;
			[[nodiscard]] stagger_axis_t get_stagger_axis() const;
			[[nodiscard]] stagger_index_t get_stagger_index() const;
			[[nodiscard]] bool get_infinite() const;

			void add_image(image_id_t image_id, sdl::surface image);
			[[nodiscard]] const sdl::surface& get_image(image_id_t image_id) const;

			[[nodiscard]] const std::vector <tiles_set>& get_tilesets() const;

			void add_tile_set(const tiles_set& ts);
			void add_animation_sequence(tile_id_t tid, const animation_sequence& aseq);
			[[nodiscard]] const std::map <tile_id_t, animation_sequence> get_animations() const;
			void add_layer(const object_layer& layer);
			void add_layer(const image_layer& layer);
			void add_layer(const tiles_layer& layer);

			[[nodiscard]] std::optional <animation_sequence> get_animation_sequence(tile_id_t tid) const;
			[[nodiscard]] bool is_inimated(tile_id_t tid) const;
			[[nodiscard]] const std::map <image_id_t, sdl::surface>& get_images() const;
			[[nodiscard]] std::map <image_id_t, sdl::surface>& get_images();

		private:
			const orientation_t m_orientation;

			const sdl::area_type m_map_dims;
			const sdl::area_type m_tile_dims;

			const sdl::color m_bgcolor;

			const render_order_t m_render_order;

			unsigned m_hex_side_length;
			stagger_axis_t m_axis;
			stagger_index_t m_index;

			bool m_infinite;

			tile_id_t m_empty_tile_id{0};

			std::map <image_id_t, sdl::surface> m_images;
			std::vector <tiles_set> m_tile_sets;
			std::map <tile_id_t, animation_sequence> m_animations;
			std::vector <layer_t> m_layers;
	};

	// ==========================================================================
	template<class Functor>
	void world::visit_layers(Functor&& visitor) const {
		for (const auto& the_layer : m_layers) {
			visitor(the_layer);
		}
	}
}

#endif
