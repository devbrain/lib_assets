//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_OBJECT_LAYER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_OBJECT_LAYER_HH

#include <vector>
#include <assets/resources/world/world_object.hh>
#include <assets/resources/world/world_component.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT object_layer : public component {
		public:
			[[nodiscard]] std::vector <object_t>::const_iterator begin() const;
			[[nodiscard]] std::vector <object_t>::const_iterator end() const;
			[[nodiscard]] bool empty() const;
			[[nodiscard]] std::size_t size() const;

			void add(const object_t& obj);
		private:
			std::vector <object_t> m_objects;
	};
}

#endif
