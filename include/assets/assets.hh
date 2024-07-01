//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_ASSETS_HH_
#define ASSETS_INCLUDE_ASSETS_ASSETS_HH_

#include <tuple>
#include <type_traits>

#include "assets/resources/image/image_data_loader.hh"
#include "assets/resources/exe/exe_data_loader.hh"
#include "assets/resources/exe/winres_data_loader.hh"
#include "assets/resources/audio/sound_effect_data_loader.hh"
#include "assets/resources/audio/music_data_loader.hh"
#include "assets/resources/font/bgi_font_data_loader.hh"
#include "assets/resources/font/ttf_data_loader.hh"
#include "assets/resources/tileset/tileset_data_loader.hh"
#include "assets/resources/world/world_data_loader.hh"

#include <bsw/mp/typelist.hh>

namespace neutrino::assets {

	namespace detail {
		template <typename X>
		struct resource_type_mapper
		{
			using type = typename X::resource_type_t;
		};

		template<typename... Loaders>
		struct data_manager_traits {
			using loaders_list_t = bsw::mp::type_list <
				image_data_loader,
				exe_data_loader,
				winres_data_loader,
				sound_effect_data_loader,
				music_data_loader,
				bgi_font_data_loader,
				ttf_data_loader,
				tileset_data_loader,
				world_data_loader,
				Loaders...
			>;
			using resources_list_t = bsw::mp::type_list_map_t<resource_type_mapper, loaders_list_t>;
			using holder_t = bsw::mp::type_list_to_tuple_t<loaders_list_t>;



			template <typename ResourceType>
			static constexpr auto get_index() {
				return bsw::mp::type_list_find_first_v<ResourceType, resources_list_t>;
			}

			template <typename ResourceType>
			static constexpr bool has_additional_arg() {
				return bsw::mp::type_list_at_t<get_index<ResourceType>(), loaders_list_t>::has_additional_arg;
			}

			template <typename ResourceType>
			using additional_arg_t = typename bsw::mp::type_list_at_t<get_index<ResourceType>(), loaders_list_t>::additional_arg_t;
		};
	}

	template<typename... Loaders>
	class data_manager {
		using my_traits = detail::data_manager_traits<Loaders...>;
		public:
			data_manager() = default;

			template <typename ResourceType>
			constexpr auto& get_data_loader() {
				return std::get<my_traits::template get_index<ResourceType>()>(m_holder);
			}

			template <typename ResourceType>
			std::enable_if_t<!my_traits::template has_additional_arg<ResourceType>(), ResourceType> load(std::istream& is) {
				return get_data_loader<ResourceType>().load(is);
			}

			template <typename ResourceType>
			std::enable_if_t<my_traits::template has_additional_arg<ResourceType>(), ResourceType> load(std::istream& is,
												const typename my_traits::template additional_arg_t<ResourceType>& arg) {
				return get_data_loader<ResourceType>().load(is, arg);
			}
		private:
			typename my_traits::holder_t m_holder;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_ASSETS_HH_
