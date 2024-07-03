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
#include <bsw/override.hh>

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

			static const std::map <std::string, assets::text::halign_t> hmp = {
				{"left", assets::text::halign_t::LEFT},
				{"right", assets::text::halign_t::RIGHT},
				{"center", assets::text::halign_t::CENTER},
				{"justify", assets::text::halign_t::JUSTIFY}
			};
			m_halign = elt.parse_enum("halign", assets::text::halign_t::LEFT, hmp);
			static const std::map <std::string, assets::text::valign_t> vmp = {
				{"center", assets::text::valign_t::CENTER},
				{"top", assets::text::valign_t::TOP},
				{"bottom", assets::text::valign_t::BOTTOM}
			};
			m_valign = elt.parse_enum("valign", assets::text::valign_t::TOP, vmp);
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

	void transform(const object& obj, assets::world_object& out) {
		out.m_id = obj.m_id;
		out.m_name = obj.m_name;
		out.m_type = obj.m_type;
		out.m_origin = obj.m_origin;
		out.m_width = obj.m_width;
		out.m_height = obj.m_height;
		out.m_rotation = obj.m_rotation;
		out.m_visible = obj.m_visible;
		out.m_gid = tile_id_t(obj.m_gid);
		out.m_hflip = obj.m_hflip;
		out.m_vflip = obj.m_vflip;
		out.m_dflip = obj.m_dflip;
		obj.assign(out);
	}

	void transform(const ellipse& obj, assets::ellipse& out) {
		transform(static_cast <const object&>(obj), out);
	}

	void transform(const point& obj, assets::point& out) {
		transform(static_cast <const object&>(obj), out);
	}

	void transform(const polyline& obj, assets::poly_line& out) {
		transform(static_cast <const object&>(obj), out);
		out.m_points = obj.m_points;
	}

	void transform(const polygon& obj, assets::polygon& out) {
		transform(static_cast <const object&>(obj), out);
		out.m_points = obj.m_points;
	}

	void transform(const text& obj, assets::text& out) {
		transform(static_cast <const object&>(obj), out);
		out.m_font_family = obj.m_font_family;
		out.m_pixel_size = obj.m_pixel_size;
		out.m_wrap = obj.m_wrap;
		out.m_color = sdl::color(obj.m_color.r, obj.m_color.g, obj.m_color.b, obj.m_color.a);
		out.m_bold = obj.m_bold;
		out.m_italic = obj.m_italic;
		out.m_underline = obj.m_underline;
		out.m_strike = obj.m_strike;
		out.m_kerning = obj.m_kerning;
		out.m_halign = obj.m_halign;
		out.m_valign = obj.m_valign;
		out.m_data = obj.m_data;

	}

	assets::object_t transform(const object_t& obj) {
		return std::visit(bsw::overload(
			                  [](const object& obj) {
				                  assets::world_object out;
				                  transform(obj, out);
				                  return assets::object_t{out};
			                  },
			                  [](const ellipse& obj) {
				                  assets::ellipse out;
				                  transform(obj, out);
				                  return assets::object_t{out};
			                  },
			                  [](const point& obj) {
				                  assets::point out;
				                  transform(obj, out);
				                  return assets::object_t{out};
			                  },
			                  [](const polygon& obj) {
				                  assets::polygon out;
				                  transform(obj, out);
				                  return assets::object_t{out};
			                  },
			                  [](const polyline& obj) {
				                  assets::poly_line out;
				                  transform(obj, out);
				                  return assets::object_t{out};
			                  },
			                  [](const text& obj) {
				                  assets::text out;
				                  transform(obj, out);
				                  return assets::object_t{out};
			                  }
		                  )
		                  , obj);
	}
}
