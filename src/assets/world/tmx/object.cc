//
// Created by igor on 27/07/2021.
//

#include "object.hh"
#include "cell.hh"
#include "json_reader.hh"
#include "xml_reader.hh"
#include <bsw/strings/string_tokenizer.hh>
#include <bsw/strings/number_parser.hh>
#include <bsw/strings/string_utils.hh>
#include <bsw/exception.hh>

namespace neutrino::assets::tmx {
	namespace {
		std::pair <float, bool> cnv(const std::string& x) {
			auto v = bsw::trim(x);
			double r = 0;
			bool status = bsw::number_parser::try_parse_float(v, r);
			return {(float)r, status};
		}

		std::vector <sdl::point2f> parse_points(const std::string& points) {
			std::vector <sdl::point2f> res;
			bsw::string_tokenizer grp(points, " ", bsw::string_tokenizer::TOK_IGNORE_EMPTY);
			for (const auto& g : grp) {
				bsw::string_tokenizer p(g, ",", bsw::string_tokenizer::TOK_IGNORE_EMPTY);
				ENFORCE(p.count () == 2);
				auto [x, s1] = cnv(p[0]);
				auto [y, s2] = cnv(p[1]);
				if (s1 && s2) {
					res.emplace_back(x, y);
				}
			}
			return res;
		}

		std::vector <sdl::point2f> parse_points(const reader& elt, const char* name = nullptr) {
			if (const auto* json_rdr = dynamic_cast <const json_reader*>(&elt); json_rdr) {
				return json_rdr->parse_points(name);
			} else {
				std::string points = elt.get_string_attribute("points");
				return parse_points(points);
			}
		}
	}

	static bool get_object_discriminator(const reader& elt, const char* name) {
		if (reader::is_json(elt)) {
			return elt.get_bool_attribute(name, false);
		}
		return elt.has_child(name);
	}

	object_t parse_object(const reader& elt) {
		try {
			auto id = elt.get_uint_attribute("id", (unsigned int)-1);
			auto name = elt.get_string_attribute("name", "");
			auto type = elt.get_string_attribute("type", "");
			auto x = elt.get_double_attribute("x", 0);
			auto y = elt.get_double_attribute("y", 0);
			auto width = elt.get_double_attribute("width", 0);
			auto height = elt.get_double_attribute("height", 0);
			auto gid = elt.get_int_attribute("gid", 0);
			auto rotation = elt.get_double_attribute("rotation", 0.0);
			bool visible = elt.get_bool_attribute("visible", true);

			sdl::point2f origin(static_cast <float>(x), static_cast <float>(y));
			auto c = cell::decode_gid(gid);
			object_attribs atts(id, name, type, origin, width, height, rotation, visible, c.gid(),
			                    c.hor_flipped(), c.vert_flipped(), c.diag_flipped());

			if (elt.has_child("polygon")) {
				polygon obj(atts);
				component::parse(obj, elt);

				if (!reader::is_json(elt)) {
					elt.parse_one_element("polygon", [&obj](const reader& e) {
						obj.points(parse_points(e));
					});
				} else {
					obj.points(parse_points(elt, "polygon"));
				}

				return obj;
			}

			if (elt.has_child("polyline")) {
				polyline obj(atts);

				component::parse(obj, elt);
				if (!reader::is_json(elt)) {
					elt.parse_one_element("polyline", [&obj](const reader& e) {
						obj.points(parse_points(e));
					});
				} else {
					obj.points(parse_points(elt, "polyline"));
				}
				return obj;
			}

			if (get_object_discriminator(elt, "ellipse")) {
				ellipse obj(atts);
				component::parse(obj, elt);
				return obj;
			}

			if (get_object_discriminator(elt, "point")) {
				point obj(atts);
				component::parse(obj, elt);

				return obj;
			}

			if (elt.has_child("text")) {
				text obj(atts);
				component::parse(obj, elt);
				elt.parse_one_element("text", [&obj](const reader& elt) {
					obj.parse(elt);
				});
				return obj;
			}

			object obj(atts);
			component::parse(obj, elt);
			return obj;
		} catch (bsw::exception& e) {
			auto id = elt.get_string_attribute("id", "<missing>");
			auto name = elt.get_string_attribute("name", "<unknown>");
			RAISE_EX_WITH_CAUSE(std::move (e), "Failed to parse object [", name, "], id [", id, "]");
		}
	}

	void text::parse(const reader& elt) {
		try {
			m_font_family = elt.get_string_attribute("fontfamily", "sans-serif");
			m_pixel_size = elt.get_int_attribute("pixelsize", 16);
			m_wrap = elt.get_bool_attribute("wrap", false);
			m_bold = elt.get_bool_attribute("bold", false);
			m_italic = elt.get_bool_attribute("italic", false);
			m_underline = elt.get_bool_attribute("underline", false);
			m_strike = elt.get_bool_attribute("strikeout", false);
			m_kerning = elt.get_bool_attribute("kerning", true);

			m_color = colori(elt.get_string_attribute("color", "#000000"));

			static const std::map <std::string, text::halign_t> hmp = {
				{"left", text::halign_t::LEFT},
				{"right", text::halign_t::RIGHT},
				{"center", text::halign_t::CENTER},
				{"justify", text::halign_t::JUSTIFY}
			};
			m_halign = elt.parse_enum("halign", text::halign_t::LEFT, hmp);
			static const std::map <std::string, text::valign_t> vmp = {
				{"center", text::valign_t::CENTER},
				{"top", text::valign_t::TOP},
				{"bottom", text::valign_t::BOTTOM}
			};
			m_valign = elt.parse_enum("valign", text::valign_t::TOP, vmp);
			if (const auto* xml_rdr = dynamic_cast <const xml_reader*>(&elt); xml_rdr) {
				m_data = xml_rdr->get_text();
			} else if (const auto* json_rdr = dynamic_cast <const json_reader*>(&elt); json_rdr) {
				m_data = json_rdr->get_string_attribute("text");
			} else {
				RAISE_EX("Not implemeted yet");
			}
		} catch (bsw::exception& e) {
			auto id = elt.get_string_attribute("id", "<missing>");
			auto name = elt.get_string_attribute("name", "<unknown>");
			RAISE_EX_WITH_CAUSE(std::move (e), "Failed to parse text object [", name, "], id [", id, "]");
		}
	}
}
