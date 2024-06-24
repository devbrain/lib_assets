//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_DATA_MANAGER_HH_

#include <istream>
#include <memory>
#include <string>
#include <map>

#include <assets/resources/abstract_resource_loader.hh>
#include <assets/resources/detail/istream_pos_keeper.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {

	template<typename Resource>
	class data_manager {
		public:
			using resource_loader_t = abstract_resource_loader <Resource>;

		public:
			data_manager() = default;
			data_manager& operator =(data_manager&) = delete;
			data_manager(const data_manager&) = delete;

			void register_loader(std::string key, std::unique_ptr <resource_loader_t>&& loader);

			Resource load(std::istream& is) const;

			Resource load(std::string key, std::istream& is) const;

		private:
			std::map <std::string, std::unique_ptr <resource_loader_t>> m_loaders;
	};

	template<typename Resource>
	void data_manager <Resource>::register_loader(std::string key,
	                                                             std::unique_ptr <resource_loader_t>&& loader) {
		ENFORCE(m_loaders.find(key) == m_loaders.end())
		m_loaders.emplace(std::make_pair(key, std::move(loader)));
	}

	template<typename Resource>
	Resource data_manager <Resource>::load(std::istream& is) const {
		for (auto i = m_loaders.begin(); i != m_loaders.end(); i++) {
			detail::istream_pos_keeper keeper(is);
			if (i->second->accept(is)) {
				keeper.rewind();
				try {
					return i->second->load(is);
				} catch (...) {}
			}
		}
		RAISE_EX("Can not find suitable loader");
	}

	template<typename Resource>
	Resource data_manager <Resource>::load(std::string key, std::istream& is) const {
		auto i = m_loaders.find(key);
		if (i == m_loaders.end()) {
			RAISE_EX("Can not find loader with key = ", key);
		}
		detail::istream_pos_keeper keeper(is);
		if (i->second->accept(is)) {
			keeper.rewind();
			try {
				return i->second->load(is);
			} catch (...) {

			}
		}
		RAISE_EX("Data is not acceptable for the loader with key = ", key);
	}
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_DATA_MANAGER_HH_
