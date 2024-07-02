//
// Created by igor on 7/2/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_COMPONENT_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_COMPONENT_HH

#include <map>
#include <string>
#include <filesystem>
#include <variant>
#include <optional>
#include <sdlpp/video/color.hh>
#include <assets/resources/world/types.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	using typed_string = std::tuple <std::string, std::string>; // < type, value >

	using property_t = std::variant <std::string,
	                                 int64_t,
	                                 bool,
	                                 float,
	                                 sdl::color,
	                                 std::filesystem::path,
	                                 object_id_t,
	                                 typed_string>;

	class ASSETS_EXPORT component {
		public:
			template<typename T>
			void add(const std::string& name, T&& v) {
				m_prop.insert(std::make_pair(name, property_t{std::forward <T>(v)}));
			}

			[[nodiscard]] bool empty() const noexcept;
			[[nodiscard]] bool contains(const std::string& name) const noexcept;
			[[nodiscard]] std::optional <property_t> get(const std::string& name) const noexcept;

		private:
			std::map <std::string, property_t> m_prop;
	};
}

#endif
