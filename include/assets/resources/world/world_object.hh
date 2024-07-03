//
// Created by igor on 7/3/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_OBJECT_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_OBJECT_HH

#include <vector>
#include <variant>
#include <assets/resources/world/world_component.hh>
#include <assets/resources/world/types.hh>
#include <assets/assets_export.h>
#include <sdlpp/video/geometry.hh>


namespace neutrino::assets {
	struct ASSETS_EXPORT world_object : component {
		unsigned m_id;
		std::string m_name;
		std::string m_type;
		sdl::point2f m_origin{0,0};
		double m_width;
		double m_height;
		double m_rotation;
		bool m_visible;
		tile_id_t m_gid{0};
		bool m_hflip;
		bool m_vflip;
		bool m_dflip;
	};

	struct ASSETS_EXPORT point : world_object {

	};

	struct ASSETS_EXPORT ellipse : world_object {

	};

	struct ASSETS_EXPORT poly_line : world_object {
		std::vector <sdl::point2f> m_points;
	};

	struct ASSETS_EXPORT polygon : poly_line {
	};

	class text : public world_object {
		public:
			enum class halign_t {
				LEFT,
				CENTER,
				RIGHT,
				JUSTIFY
			};

			enum class valign_t {
				TOP,
				CENTER,
				BOTTOM
			};


			std::string m_font_family;
			int m_pixel_size{};
			bool m_wrap{};
			sdl::color m_color;
			bool m_bold{};
			bool m_italic{};
			bool m_underline{};
			bool m_strike{};
			bool m_kerning{};
			halign_t m_halign;
			valign_t m_valign;
			std::string m_data;
	};

	using object_t = std::variant<world_object, ellipse, point, poly_line, polygon, text>;
}

#endif
