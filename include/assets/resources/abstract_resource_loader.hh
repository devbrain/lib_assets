//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_

#include <iosfwd>

namespace neutrino::assets {

	namespace detail {
		template <typename ArgType>
		struct arg_type_traits {
			using type = const ArgType&;
		};

		template <>
		struct arg_type_traits<std::istream> {
			using type = std::istream&;
		};
	}


	template<typename Resource, typename ArgType = std::istream>
	class abstract_resource_loader {
		public:
			static constexpr bool has_additional_arg = false;
			using resource_type_t = Resource;
			using arg_type_t = ArgType;
			using param_type = typename detail::arg_type_traits<ArgType>::type;


			virtual ~abstract_resource_loader() = default;
			virtual bool accept(param_type is) const = 0;
			virtual Resource load(param_type is) const = 0;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_
