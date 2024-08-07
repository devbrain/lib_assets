//
// Created by igor on 06/08/2021.
//

#include <limits>
#include "reader.hh"
#include "json_reader.hh"
#include <bsw/exception.hh>
#include <bsw/strings/number_parser.hh>
#include <bsw/strings/ascii.hh>

namespace neutrino::assets::tmx {
	reader::document_t reader::guess_document_type(const char* txt, std::size_t size) {
		std::size_t k = 0;
		for (k = 0; k < size; k++) {
			auto ch = txt[k];
			if (!bsw::ascii::is_space(ch)) {
				if (ch == '<') {
					return XML_DOCUMENT;
				} else {
					if (ch == '{' || ch == '[') {
						return JSON_DOCUMENT;
					} else {
						return UNKNOWN_TYPE;
					}
				}
			}
		}
		return UNKNOWN_TYPE;
	}

	bool reader::is_json(const reader& elt) noexcept {
		return dynamic_cast <const json_reader*>(&elt) != nullptr;
	}

	reader::~reader() = default;

	unsigned reader::get_uint_attribute(const char* name) const {
		auto a = get_attribute_value(name);
		if (!a) {
			RAISE_EX("Can not find attribute ", name);
		}
		return bsw::number_parser::parse_unsigned(*a);
	}

	unsigned reader::get_uint_attribute(const char* name, unsigned val) const {
		auto a = get_attribute_value(name);
		if (!a) {
			return val;
		}
		return bsw::number_parser::parse_unsigned(*a);
	}

	int reader::get_int_attribute(const char* name) const {
		auto a = get_attribute_value(name);
		if (!a) {
			RAISE_EX("Can not find attribute ", name);
		}
		return bsw::number_parser::parse(*a);
	}

	int reader::get_int_attribute(const char* name, int val) const {
		auto a = get_attribute_value(name);
		if (!a) {
			return val;
		}
		auto i64 = bsw::number_parser::parse64(*a);
		if (i64 > std::numeric_limits<int>::max()) {
			return std::numeric_limits<int>::max();
		}
		return static_cast <int>(i64);
	}

	double reader::get_double_attribute(const char* name) const {
		auto a = get_attribute_value(name);
		if (!a) {
			RAISE_EX("Can not find attribute ", name);
		}
		return bsw::number_parser::parse_float(*a);
	}

	double reader::get_double_attribute(const char* name, double val) const {
		auto a = get_attribute_value(name);
		if (!a) {
			return val;
		}
		return bsw::number_parser::parse_float(*a);
	}

	bool reader::get_bool_attribute(const char* name) const {
		auto a = get_attribute_value(name);
		if (!a) {
			RAISE_EX("Can not find attribute ", name);
		}
		return bsw::number_parser::parse_bool(*a);
	}

	bool reader::get_bool_attribute(const char* name, bool val) const {
		auto a = get_attribute_value(name);
		if (!a) {
			return val;
		}
		return bsw::number_parser::parse_bool(*a);
	}

	std::string reader::get_string_attribute(const char* name) const {
		auto a = get_attribute_value(name);
		if (!a) {
			RAISE_EX("Can not find attribute ", name);
		}
		return *a;
	}

	std::string reader::get_string_attribute(const char* name, const char* val) const {
		auto a = get_attribute_value(name);
		if (!a) {
			return val;
		}
		return *a;
	}

	bool reader::has_attribute(const char* name) const {
		auto a = get_attribute_value(name);
		return a.has_value();
	}
}
