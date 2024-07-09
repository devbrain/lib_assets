//
// Created by igor on 6/28/24.
//
#include <limits>
#include <assets/resources/tileset/tileset_data_loader.hh>
#include "win_icon_loader.hh"
#include "prographx_loader.hh"

namespace neutrino::assets {
	tileset_resource::tileset_resource(std::istream& is)
		: m_stream(is) {
	}

	std::istream& tileset_resource::get_stream() const {
		return m_stream;
	}

	prographx_resource::prographx_resource(std::istream& is, bool masked, std::size_t padding) : tileset_resource(is),
		  m_masked(masked),
		  m_padding(padding),
		  m_max_tiles(std::numeric_limits<std::size_t>::max()) {
	}

	prographx_resource::prographx_resource(std::istream& is, bool masked, std::size_t padding, std::size_t max_tiles)
		: tileset_resource(is),
		  m_masked(masked),
		  m_padding(padding),
		  m_max_tiles(max_tiles) {
	}

	bool prographx_resource::is_masked() const {
		return m_masked;
	}

	std::size_t prographx_resource::get_padding() const {
		return m_padding;
	}

	std::size_t prographx_resource::get_max_tiles() const {
		return m_max_tiles;
	}

	tileset_data_loader::tileset_data_loader() {
		register_loader("MSICO", std::make_unique <win_icon_loader>());
		register_loader("PROGRAPHX", std::make_unique <prographx_loader>());
	}
}
