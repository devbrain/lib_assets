//
// Created by igor on 02/08/2021.
//

#ifndef NEUTRINO_GROUP_HH
#define NEUTRINO_GROUP_HH

#include <optional>
#include <tuple>
#include "world/tmx/reader.hh"
#include "world/tmx/component.hh"
#include "world/tmx/color.hh"

namespace neutrino::assets::tmx {
	struct group : component {
		static constexpr int default_offset_x = 0;
		static constexpr int default_offset_y = 0;
		static constexpr bool default_visible = true;

		std::optional <int> offsetx;
		std::optional <int> offsety;
		std::optional <float> opacity;
		std::optional <bool> visible;
		std::optional <colori> tint;

		static group parse(const reader& elt, const group* parent = nullptr);
		// name, offsetx, offsety, opacity, visible, tint, id
		static std::tuple <std::string, int, int, float, bool, colori, int>
		parse_content(const reader& elt, const group* self);
	};
}

#endif //NEUTRINO_GROUP_HH
