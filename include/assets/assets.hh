//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_ASSETS_HH_
#define ASSETS_INCLUDE_ASSETS_ASSETS_HH_

#include <tuple>

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
	}

	template<typename... Loaders>
	class data_manager {
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
		using resources_list_t = bsw::mp::type_list_map_t<detail::resource_type_mapper, loaders_list_t>;
		using holder_t = bsw::mp::type_list_to_tuple_t<loaders_list_t>;
		public:
			data_manager() = default;

			template <typename ResourceType>
			constexpr data_loader<ResourceType>& get_data_loader() {
				return std::get<bsw::mp::type_list_find_first_v<ResourceType, resources_list_t>>(m_holder);
			}

			template <typename ResourceType>
			ResourceType load(std::istream& is) {
				return get_data_loader<ResourceType>().load(is);
			}
		private:
			holder_t m_holder;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_ASSETS_HH_
