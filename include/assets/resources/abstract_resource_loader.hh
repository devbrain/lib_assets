//
// Created by igor on 5/28/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_

#include <iosfwd>


namespace neutrino::assets {
	namespace detail {
		struct no_args;

		template<typename Resource, typename AdditionalArg>
		class abstract_resource_loader_base {
			public:
			static constexpr bool has_additional_arg = true;
			using resource_type_t = Resource;
			using arg_type_t = AdditionalArg;

			public:
			virtual ~abstract_resource_loader_base() = default;
			virtual bool accept(std::istream& is) const = 0;

			virtual Resource load(std::istream& is, const AdditionalArg& arg) const = 0;
		};

		template<typename Resource>
		class abstract_resource_loader_base<Resource, no_args> {
			public:
			static constexpr bool has_additional_arg = false;
			using resource_type_t = Resource;
			using arg_type_t = no_args;

			public:
			virtual ~abstract_resource_loader_base() = default;
			virtual bool accept(std::istream& is) const = 0;

			virtual Resource load(std::istream& is) const = 0;
		};
	} // ns detail
	template<typename Resource, typename AdditionalArg = detail::no_args>
	class abstract_resource_loader : public detail::abstract_resource_loader_base<Resource, AdditionalArg> {

	};

}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_HH_
