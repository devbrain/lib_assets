//
// Created by igor on 19/07/2021.
//

#ifndef NEUTRINO_COMPONENT_HH
#define NEUTRINO_COMPONENT_HH

#include <map>
#include <string>
#include <filesystem>
#include <variant>
#include <optional>


#include "world/tmx/reader.hh"
#include "world/tmx/color.hh"
#include <assets/resources/world/types.hh>
#include <assets/resources/world/world_component.hh>

namespace neutrino::assets::tmx {
	using typed_string = std::tuple <std::string, std::string>; // < type, value >

	using property_t = std::variant <std::string,
	                                 int64_t,
	                                 bool,
	                                 float,
	                                 colori,
	                                 std::filesystem::path,
	                                 object_id_t,
	                                 typed_string>;

	class component {
		public:
			static void parse(component& obj, const reader& elt, const component* parent = nullptr);

			template<typename T>
			void add(const std::string& name, T&& v) {
				m_prop.insert(std::make_pair(name, property_t{std::forward <T>(v)}));
			}

			[[nodiscard]] bool empty() const noexcept;

			[[nodiscard]] bool contains(const std::string& name) const noexcept;

			[[nodiscard]] std::optional <property_t> get(const std::string& name) const noexcept;


			void assign(assets::component& out) const;
		private:
			std::map <std::string, property_t> m_prop;
	};
}

#endif //NEUTRINO_COMPONENT_HH
