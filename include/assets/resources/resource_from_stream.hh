//
// Created by igor on 7/8/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_FROM_STREAM_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_RESOURCE_FROM_STREAM_HH

#include <iosfwd>
#include <assets/resources/loader_state.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT resource_from_stream {
		public:
			virtual ~resource_from_stream();
			[[nodiscard]] virtual std::istream& get_stream() const = 0;
	};

#define ASSETS_ENABLE_ISTREAM_STATE(TYPE)												\
	template <>																			\
	struct loader_state_traits<TYPE> {													\
		using type = std::streampos;													\
		using loader_type = TYPE;														\
																						\
		static type get_state(loader_type& arg) {										\
			return loader_state_traits<std::istream>::get_state(arg.get_stream());		\
		}																				\
																						\
		static void set_state(loader_type& arg, const type& s) {						\
			loader_state_traits<std::istream>::set_state(arg.get_stream(), s);			\
		}																				\
	}

	ASSETS_ENABLE_ISTREAM_STATE(resource_from_stream);
}

#endif
