//
// Created by igor on 7/8/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_LOADER_STATE_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_LOADER_STATE_HH

#include <istream>

namespace neutrino::assets {
	template <typename LoaderType>
	struct loader_state_traits;

	template <>
	struct loader_state_traits<std::istream> {
		using type = std::streampos;
		using loader_type = std::istream;

		static type get_state(loader_type& arg) {
			return arg.tellg();
		}

		static void set_state(loader_type& arg, const type& s) {
			arg.seekg(s, std::ios::beg);
		}
	};

	namespace detail {
		template <typename LoaderType>
		struct state_keeper {
			using state_t = typename loader_state_traits<LoaderType>::type;

			LoaderType& obj;
			state_t state;

			explicit state_keeper(LoaderType& subj)
				: obj(subj), state(loader_state_traits<LoaderType>::get_state(subj)) {}

			explicit state_keeper(const LoaderType& subj)
				: obj(const_cast<LoaderType&>(subj)), state(loader_state_traits<LoaderType>::get_state(const_cast<LoaderType&>(subj))) {}


			void rewind() {
				loader_state_traits<LoaderType>::set_state(obj, state);
			}
			~state_keeper() {
				rewind();
			}
		};
	}
}

#endif
