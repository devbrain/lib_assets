//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_DATA_LOADER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_DATA_LOADER_HH_

#include <istream>
#include <memory>
#include <string>
#include <map>

#include <assets/resources/abstract_resource_loader.hh>
#include <assets/resources/loader_state.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {
	template<typename Resource, typename ArgType = std::istream>
	class data_loader {
		public:
			using resource_loader_t = abstract_resource_loader <Resource, ArgType>;
			using resource_type_t = Resource;
			static constexpr bool has_additional_arg = resource_loader_t::has_additional_arg;
			using additional_arg_t = ArgType;
			using param_type = typename resource_loader_t::param_type;

		public:
			data_loader() = default;
			data_loader& operator =(data_loader&) = delete;
			data_loader(const data_loader&) = delete;

			void register_loader(std::string key, std::unique_ptr <resource_loader_t>&& loader);

			Resource load(param_type arg) const;
			Resource load(std::string key, param_type arg) const;

		private:
			std::map <std::string, std::unique_ptr <resource_loader_t>> m_loaders;
	};

	template<typename Resource, typename ArgType>
	void data_loader <Resource, ArgType>::register_loader(std::string key,
	                                                      std::unique_ptr <resource_loader_t>&& loader) {
		ENFORCE(m_loaders.find(key) == m_loaders.end())
		m_loaders.emplace(std::make_pair(key, std::move(loader)));
	}

	template<typename Resource, typename ArgType>
	Resource data_loader <Resource, ArgType>::load(param_type arg) const {
		for (auto i = m_loaders.begin(); i != m_loaders.end(); ++i) {
			detail::state_keeper<ArgType> keeper(arg);
			if (i->second->accept(arg)) {
				keeper.rewind();
				try {
					return i->second->load(arg);
				} catch (bsw::exception& e) {
					e.trace().print();
				}
			}
		}
		RAISE_EX("Can not find suitable loader");
	}

	template<typename Resource, typename ArgType>
	Resource data_loader <Resource, ArgType>::load(std::string key, param_type arg) const {
		auto i = m_loaders.find(key);
		if (i == m_loaders.end()) {
			RAISE_EX("Can not find loader with key = ", key);
		}
		detail::state_keeper<ArgType> keeper(arg);
		if (i->second->accept(arg)) {
			keeper.rewind();
			try {
				return i->second->load(arg);
			} catch (...) {
			}
		}
		RAISE_EX("Data is not acceptable for the loader with key = ", key);
	}
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_DATA_LOADER_HH_
