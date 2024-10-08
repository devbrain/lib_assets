//
// Created by igor on 19/07/2021.
//

#include "component.hh"
#include "xml_reader.hh"
#include "json_reader.hh"
#include <sdlpp/video/color.hh>
#include <bsw/strings/number_parser.hh>
#include <bsw/switch_by_string.hh>
#include <bsw/override.hh>

namespace neutrino::assets::tmx {
	bool component::contains(const std::string& name) const noexcept {
		return m_prop.find(name) != m_prop.end();
	}

	bool component::empty() const noexcept {
		return m_prop.empty();
	}

	std::optional <property_t> component::get(const std::string& name) const noexcept {
		auto i = m_prop.find(name);
		if (i == m_prop.end()) {
			return std::nullopt;
		}
		return i->second;
	}
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++20-extensions"
#endif
	void component::assign(assets::component& out) const {
		for (const auto& [k, v] : m_prop) {
			std::visit(bsw::overload(
					[&] (const colori& ci) {
						out.add(k, sdl::color(ci.r, ci.g, ci.b, ci.a));
					},
					[&] (const auto& x) {
						out.add(k, x);
					}
				), v);
		}
	}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
	static void add_propery(component& obj, const std::string& name, const std::string& type,
	                        const std::string& value) {
		try {
			switch (switcher(type.c_str())) {
				case "bool"_case:
					obj.add(name, bsw::number_parser::parse_bool(value));
					break;
				case "int"_case:
					obj.add(name, bsw::number_parser::parse64(value));
					break;
				case "float"_case:
					obj.add(name, static_cast <float>(bsw::number_parser::parse_float(value)));
					break;
				case "color"_case:
					obj.add(name, colori(value));
					break;
				case "file"_case:
					obj.add(name, std::filesystem::path(value.empty() ? "." : value));
					break;
				case "object"_case:
					obj.add(name, object_id_t(bsw::number_parser::parse(value)));
					break;
				case "string"_case:
					obj.add(name, value);
					break;
				default:
					obj.add(name, typed_string{type, value});
					break;
			}
		} catch (bsw::exception& e) {
			RAISE_EX_WITH_CAUSE(std::move (e), "Failed to parse property [", name, "] of type [", type, "]");
		}
	}

	void component::parse(component& obj, const reader& elt, const component* parent) {
		if (parent) {
			for (const auto& [name, val] : parent->m_prop) {
				obj.add(name, val);
			}
		}
		if (const auto* json_rdr = dynamic_cast <const json_reader*>(&elt); json_rdr) {
			elt.parse_many_elements("properties", [&obj](const reader& e) {
				std::string name = e.get_string_attribute("name");
				ENFORCE(!name.empty ());
				std::string value = e.get_string_attribute("value", "");
				std::string type = e.get_string_attribute("type", "string");
				if (type != "string") {
					if (value.empty()) {
						RAISE_EX("Empty property value for non string type");
					}
				}
				add_propery(obj, name, type, value);
			});
		} else {
			elt.parse_one_element("properties", [&obj](const reader& e) {
				e.parse_many_elements("property", [&obj](const reader& inner) {
					std::string name = inner.get_string_attribute("name");
					ENFORCE(!name.empty ());
					std::string value = inner.get_string_attribute("value", "");

					if (const auto* xml_rdr = dynamic_cast <const xml_reader*>(&inner); xml_rdr && value.empty()) {
						value = xml_rdr->get_text();
					}

					std::string type = inner.get_string_attribute("type", "string");
					if (type != "string") {
						if (value.empty()) {
							RAISE_EX("Empty property value for non string type");
						}
					}
					add_propery(obj, name, type, value);
				});
			});
		}
	}
}
