//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_

#include <iosfwd>

namespace assets {
	template <typename Resource, typename MetaData>
	class abstract_resource_loader {
	 public:
		virtual ~abstract_resource_loader() = default;
		virtual bool accept(std::istream& is) const = 0;
		virtual Resource load(std::istream& is) const = 0;
		virtual MetaData info(std::istream & is) const = 0;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_
