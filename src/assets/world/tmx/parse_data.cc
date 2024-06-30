//
// Created by igor on 26/07/2021.
//

#include <sstream>
#include "parse_data.hh"
#include "csv/parser.hpp"

#include <bsw/io/base64.hh>
#include <bsw/io/inflating_stream.hh>
#include <bsw/strings/string_utils.hh>
#include <bsw/strings/number_parser.hh>
#include <bsw/switch_by_string.hh>
#include <bsw/exception.hh>

static void decode_csv(const std::string& data, std::vector <int>& out) {
	using namespace aria::csv;
	std::istringstream is(data);
	// use heap here, because CSV parser abuses stack
	std::unique_ptr <CsvParser> parser = std::make_unique <CsvParser>(is);

	for (;;) {
		auto field = parser->next_field();
		if (field.type == FieldType::DATA) {
			auto val = bsw::trim(*field.data);
			if (!val.empty()) {
				auto v = bsw::number_parser::parse_unsigned(bsw::trim(*field.data));
				out.push_back(v);
			}
		} else {
			if (field.type == FieldType::CSV_END) {
				break;
			}
		}
	}
}

namespace neutrino::assets::tmx {
	parsed_buf_t parse_data(const std::string& encoding, const std::string& compression, const std::string& data) {
		if (encoding == "csv" || encoding.empty()) {
			if (!compression.empty()) {
				RAISE_EX("compression ", compression, " is specified for CSV data");
			}
			std::vector <int> res;
			decode_csv(data, res);
			return res;
		} else {
			if (encoding == "base64") {
				std::istringstream is(bsw::trim(data));
				bsw::io::base64_decoder decoder(is);
				std::vector <char> res;
				if (!compression.empty()) {
					auto type = bsw::io::compression_type_t::ZLIB;
					switch (switcher(compression.c_str())) {
						case "gzip"_case:
							type = bsw::io::compression_type_t::GZIP;
							break;
						case "zlib"_case:
							type = bsw::io::compression_type_t::ZLIB;
							break;
						case "zstd"_case:
							type = bsw::io::compression_type_t::ZSTD;
							break;
						default:
							RAISE_EX("Unsupported compression type ", compression);
					}
					bsw::io::inflating_input_stream inflating_input_stream(decoder, type);
					res = std::vector <char>((std::istreambuf_iterator <char>(inflating_input_stream)),
					                         std::istreambuf_iterator <char>());
				} else {
					res = std::vector <char>((std::istreambuf_iterator <char>(decoder)),
					                         std::istreambuf_iterator <char>());
				}
				return res;
			} else {
				RAISE_EX("Unknown encoding ", encoding);
			}
		}
	}
}
