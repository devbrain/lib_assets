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
#include <assets/resources/detail/istream_pos_keeper.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {
	namespace detail {
		template<typename Resource, typename AdditionalArg>
		class data_loader_base {
			public:
				using resource_loader_t = abstract_resource_loader <Resource, AdditionalArg>;
				using resource_type_t = Resource;
				static constexpr bool has_additional_arg = resource_loader_t::has_additional_arg;
				using additional_arg_t = AdditionalArg;

			public:
				data_loader_base() = default;
				data_loader_base& operator =(data_loader_base&) = delete;
				data_loader_base(const data_loader_base&) = delete;

				void register_loader(std::string key, std::unique_ptr <resource_loader_t>&& loader);

				Resource load(std::istream& is, const AdditionalArg& arg) const;
				Resource load(std::string key, std::istream& is, const AdditionalArg& arg) const;

			private:
				std::map <std::string, std::unique_ptr <resource_loader_t>> m_loaders;
		};

		template<typename Resource>
		class data_loader_base <Resource, no_args> {
			public:
				using resource_loader_t = abstract_resource_loader <Resource, no_args>;
				using resource_type_t = Resource;
				static constexpr bool has_additional_arg = resource_loader_t::has_additional_arg;
				using additional_arg_t = no_args;

			public:
				data_loader_base() = default;
				data_loader_base& operator =(data_loader_base&) = delete;
				data_loader_base(const data_loader_base&) = delete;

				void register_loader(std::string key, std::unique_ptr <resource_loader_t>&& loader);

				Resource load(std::istream& is) const;
				Resource load(std::string key, std::istream& is) const;

			private:
				std::map <std::string, std::unique_ptr <resource_loader_t>> m_loaders;
		};
	} // ns detail
	template<typename Resource, typename AdditionalArg = detail::no_args>
	class data_loader : public detail::data_loader_base <Resource, AdditionalArg> {
	};

	namespace detail {
		template<typename Resource, typename AdditionalArg>
		void data_loader_base <Resource, AdditionalArg>::register_loader(std::string key,
		                                                                 std::unique_ptr <resource_loader_t>&& loader) {
			ENFORCE(m_loaders.find(key) == m_loaders.end())
			m_loaders.emplace(std::make_pair(key, std::move(loader)));
		}

		template<typename Resource, typename AdditionalArg>
		Resource data_loader_base <Resource, AdditionalArg>::load(std::istream& is, const AdditionalArg& arg) const {
			for (auto i = m_loaders.begin(); i != m_loaders.end(); ++i) {
				istream_pos_keeper keeper(is);
				if (i->second->accept(is)) {
					keeper.rewind();
					try {
						return i->second->load(is, arg);
					}
					catch (...) {
					}
				}
			}
			RAISE_EX("Can not find suitable loader");
		}

		template<typename Resource, typename AdditionalArg>
		Resource data_loader_base <Resource, AdditionalArg>::load(std::string key, std::istream& is,
		                                                          const AdditionalArg& arg) const {
			auto i = m_loaders.find(key);
			if (i == m_loaders.end()) {
				RAISE_EX("Can not find loader with key = ", key);
			}
			istream_pos_keeper keeper(is);
			if (i->second->accept(is)) {
				keeper.rewind();
				try {
					return i->second->load(is, arg);
				} catch (...) {
				}
			}
			RAISE_EX("Data is not acceptable for the loader with key = ", key);
		}

		// -------------------------------------------------
		template<typename Resource>
		void data_loader_base <Resource, no_args>::register_loader(std::string key,
		                                                           std::unique_ptr <resource_loader_t>&& loader) {
			ENFORCE(m_loaders.find(key) == m_loaders.end())
			m_loaders.emplace(std::make_pair(key, std::move(loader)));
		}

		template<typename Resource>
		Resource data_loader_base <Resource, no_args>::load(std::istream& is) const {
			for (auto i = m_loaders.begin(); i != m_loaders.end(); ++i) {
				istream_pos_keeper keeper(is);
				if (i->second->accept(is)) {
					keeper.rewind();
					try {
						return i->second->load(is);
					} catch (...) {
					}
				}
			}
			RAISE_EX("Can not find suitable loader");
		}

		template<typename Resource>
		Resource data_loader_base <Resource, no_args>::load(std::string key, std::istream& is) const {
			auto i = m_loaders.find(key);
			if (i == m_loaders.end()) {
				RAISE_EX("Can not find loader with key = ", key);
			}
			istream_pos_keeper keeper(is);
			if (i->second->accept(is)) {
				keeper.rewind();
				try {
					return i->second->load(is);
				} catch (...) {
				}
			}
			RAISE_EX("Data is not acceptable for the loader with key = ", key);
		}
	} // ns detail
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_DATA_LOADER_HH_
