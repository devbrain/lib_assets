//
// Created by igor on 7/5/24.
//

#ifndef  WORLD_MODEL_HH
#define  WORLD_MODEL_HH

#include <map>
#include <variant>
#include <list>

#include <assets/resources/world/world.hh>
#include <world_renderer/texture_atlas.hh>
#include <world_renderer/color_layer.hh>
#include <world_renderer/image_layer.hh>
#include <world_renderer/tiles_layer.hh>
#include <world_renderer/animation_sequence.hh>

namespace neutrino::tiled {
	class world_model {
		public:
			world_model();

			template<typename T>
			world_model& append(T&& layer) {
				m_layers.emplace_back(layer);
				return *this;
			}

			template<typename T>
			world_model& prepend(T&& layer) {
				m_layers.emplace_front(layer);
				return *this;
			}

			template<typename T>
			world_model& insert(std::size_t pos, T&& layer) {
				auto itr = m_layers.begin();
				for (auto i = 0u; i <= pos && itr != m_layers.end(); i++, itr++);
				m_layers.insert(itr, layer);
				return *this;
			}

			void add_animation(const tile& base_tile, const tile& tile_, const std::chrono::milliseconds& duration);
			[[nodiscard]] const std::map <tile, animation_sequence>& get_animations() const;

			using layer_t = std::variant <color_layer, image_layer, tiles_layer>;
			using container_t = std::list <layer_t>;

			[[nodiscard]] container_t::const_iterator begin() const;
			[[nodiscard]] container_t::const_iterator end() const;

			void set_geometry(unsigned tile_width, unsigned tile_height, unsigned num_tiles_x, unsigned num_tiles_y);

			[[nodiscard]] world_coords_t get_world_width() const;
			[[nodiscard]] world_coords_t get_world_height() const;
			[[nodiscard]] world_coords_t get_tile_width() const;
			[[nodiscard]] world_coords_t get_tile_height() const;

		private:
			texture_atlas m_atlas;

			std::list <layer_t> m_layers;
			std::map <tile, animation_sequence> m_animations;

			world_coords_t m_world_width;
			world_coords_t m_world_height;
			world_coords_t m_tile_width;
			world_coords_t m_tile_height;
	};
}

#endif
