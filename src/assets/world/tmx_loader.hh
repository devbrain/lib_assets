//
// Created by igor on 7/2/24.
//

#ifndef  TMX_LOADER_HH
#define  TMX_LOADER_HH

#include <iosfwd>

#include "world/path_resolver.hh"
#include <assets/resources/world/world.hh>
#include <assets/resources/image/image_data_loader.hh>

namespace neutrino::assets::tmx {
	world load (std::istream& is, const path_resolver_t& resolver, const data_loader<sdl::surface>& image_loader);
}






#endif
