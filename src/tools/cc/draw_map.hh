//
// Created by igor on 7/12/24.
//

#ifndef  DRAW_MAP_HH
#define  DRAW_MAP_HH


#include "map_tile.hh"
#include <assets/assets.hh>
#include <sdlpp/video/surface.hh>

neutrino::sdl::surface draw_map(std::tuple <bg_map_t, fg_map_t>& mp, const neutrino::assets::tileset& ts, int w, int h);




#endif
