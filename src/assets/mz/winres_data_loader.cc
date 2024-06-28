//
// Created by igor on 6/26/24.
//

#include <assets/resources/exe/winres_data_loader.hh>
#include "windows_resources_loader.hh"

namespace neutrino::assets {
	winres_data_loader::winres_data_loader() {
		register_loader("EXE", std::make_unique<windows_resources_loader>());
	}
}
