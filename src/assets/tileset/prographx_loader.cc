//
// Created by igor on 7/7/24.
//

#include "prographx_loader.hh"
#include <bsw/io/binary_reader.hh>


namespace neutrino::assets {
	bool prographx_loader::accept(std::istream& is) const {
		bsw::io::binary_reader rdr(is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
		auto pos = is.tellg();
		is.seekg(0, std::ios::end);
		auto fsize = is.tellg() - pos;
		is.seekg(pos, std::ios::beg);
		if (fsize <= 3) {
			return false;
		}

		while (true) {
			uint8_t numTiles, widthBytes, height;
			rdr >> numTiles >> widthBytes >> height;
		//	unsigned int lenTile = widthBytes * height * (unsigned int)PlaneCount::Masked;
		}
	}

	tileset prographx_loader::load(std::istream& is) const {
	}
}
