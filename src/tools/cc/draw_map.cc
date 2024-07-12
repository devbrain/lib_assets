//
// Created by igor on 7/12/24.
//

#include "draw_map.hh"

using namespace neutrino;

void draw_layer(sdl::surface& srf, const std::vector<map_tile>& m, const assets::tileset& ts) {
	const auto& tiles = ts.get_surface();
	for (const auto& tile : m) {
		auto src_rect = ts.get_tile(assets::tile_id_t(tile.code));

		sdl::rect dst_rect(tile.posx*src_rect.w, tile.posy*src_rect.h, src_rect.w, src_rect.h);
		tiles.blit(src_rect, srf, dst_rect);
	}
}

sdl::surface draw_map(std::tuple <bg_map_t, fg_map_t>& mp, const assets::tileset& ts, int w, int h) {
	sdl::surface srf = sdl::surface::make_8bit(w*16, h*16);
	srf.set_palette(assets::load_standard_vga_palette());
	draw_layer(srf, std::get<0>(mp), ts);
	draw_layer(srf, std::get<1>(mp), ts);
	return srf;
}