//
// Created by igor on 7/5/24.
//

#ifndef  WORLD_LOADER_HH
#define  WORLD_LOADER_HH


#include <tuple>
#include <assets/resources/world/world.hh>
#include <world_renderer/world_model.hh>
#include <world_renderer/atlas_builder.hh>

namespace neutrino::tiled {



	std::tuple<atlas_builder, world_model> world_loader(assets::world&& w);

}



#endif
