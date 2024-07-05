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
			world_model() = default;

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

		private:
			texture_atlas m_atlas;
			using layer_t = std::variant <color_layer, image_layer, tiles_layer>;
			std::list <layer_t> m_layers;
			std::map <tile, animation_sequence> m_animations;
	};
}

#endif
