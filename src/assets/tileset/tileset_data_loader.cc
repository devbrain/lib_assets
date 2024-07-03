//
// Created by igor on 6/28/24.
//

#include <assets/resources/tileset/tileset_data_loader.hh>
#include "win_icon_loader.hh"

namespace neutrino::assets {
	tileset_data_loader::tileset_data_loader() {
		register_loader("MSICO", std::make_unique<win_icon_loader>());
	}
}