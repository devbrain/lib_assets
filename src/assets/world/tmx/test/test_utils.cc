//
// Created by igor on 24/07/2021.
//

#include "test_utils.hh"
#include <sstream>
#include <bsw/io/memory_stream_buf.hh>
#include <utility>
#include "world/tmx/xml_reader.hh"
#include "world/tmx/json_reader.hh"

namespace neutrino::assets::tmx::test {
	map load_map(const std::string& txt, path_resolver_t resolver) {
		return load_map(reinterpret_cast <const unsigned char*>(txt.c_str()), txt.size(), resolver);
	}

	map load_map(const unsigned char* data, std::size_t length, path_resolver_t resolver) {
		reader::document_t doc_type;
		doc_type = reader::guess_document_type((char*)data, length);
		switch (doc_type) {
			case reader::XML_DOCUMENT:
				return map::parse(xml_reader::load((char*)data, length, "map"), std::move(resolver));
			case reader::JSON_DOCUMENT:
				return map::parse(json_reader::load((char*)data, length, nullptr), std::move(resolver));
			default:
				RAISE_EX("Unknown document type");
		}
	}

	// ---------------------------------------------------------------------------------
	bool check_properties(const component& obj, const std::map <std::string, property_t>& props) {
		for (const auto& [name, val] : props) {
			if (auto pprop = obj.get(name); pprop.has_value()) {
				if (*pprop != val) {
					return false;
				}
			} else {
				return false;
			}
		}
		return true;
	}

	// ---------------------------------------------------------------------------------
	bool test_tiles(const tile_layer& tl, const std::vector <int>& expected) {
		std::size_t k = 0;
		for (const auto c : tl.cells()) {
			if (k >= expected.size()) {
				return false;
			}
			if (c.gid() != (unsigned int)expected[k++]) {
				return false;
			}
		}
		return (k == expected.size());
	}

	bool eq_cells(const std::vector <cell>& a, const std::vector <cell>& b) {
		if (a.size() != b.size()) {
			return false;
		}
		for (std::size_t i = 0; i < a.size(); i++) {
			if (a[i].hor_flipped() != b[i].hor_flipped()) {
				return false;
			}
			if (a[i].vert_flipped() != b[i].vert_flipped()) {
				return false;
			}
			if (a[i].diag_flipped() != b[i].diag_flipped()) {
				return false;
			}
			if (a[i].gid() != b[i].gid()) {
				return false;
			}
		}
		return true;
	}
}
