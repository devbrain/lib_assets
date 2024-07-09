//
// Created by igor on 7/9/24.
//

#include <istream>
#include <bsw/exception.hh>
#include <assets/resources/image/ega_image.hh>
#include <assets/resources/palette/palette_data_loader.hh>
#include "raw_image_loader.hh"


namespace neutrino::assets {
	bool raw_image_loader::accept(const image_resource& res) const {
		if (const auto* rc = dynamic_cast<const raw_image_resource*>(&res)) {
			auto& is = res.get_stream();
			std::size_t pos = is.tellg();
			is.seekg(0, std::ios::end);
			std::size_t fsize = static_cast<std::size_t>(is.tellg()) - pos;
			if (rc->get_image_type() == raw_image_resource::EGA) {
				return fsize == 320*200/2;
			} else {
				if (rc->where_palette() == raw_image_resource::NO_PALETTE) {
					return fsize == 320*200;
				} else {
					return fsize == 320*200 + 768;
				}
			}
		}
		return false;
	}

	static sdl::surface create_surface(const pixels& px, const sdl::palette& pal) {
		unsigned w = px.get_width();
		unsigned h = px.get_height();
		sdl::surface srf = sdl::surface::make_8bit(w, h);
		srf.set_palette(pal);
		for (unsigned y=0; y<h; y++) {
			uint8_t* row = static_cast <uint8_t*>(srf->pixels) + y*srf->pitch;
			for (unsigned x=0; x<w; x++) {
				uint8_t* target = row + x;
				*target = *(px.data() + y*w + x);
			}
		}
		return srf;
	}

	sdl::surface raw_image_loader::load(const image_resource& res) const {
		if (const auto* rc = dynamic_cast<const raw_image_resource*>(&res)) {
			if (rc->get_image_type() == raw_image_resource::EGA) {
				const auto descr = ega_byte_description{
					ega_bit_purpose::red1,
					ega_bit_purpose::green1,
					ega_bit_purpose::blue1,
					ega_bit_purpose::intensity1,
				};
				unsigned w = 320;
				unsigned h = 200;
				const auto [material, _] = load_planar_ega(rc->get_stream(), descr, w, h);
				return create_surface(material, load_standard_ega_palette());
			} else {
				sdl::palette pal;
				palette_data_loader dm;
				if (rc->where_palette() == raw_image_resource::PALLETE_AT_START) {
					pal = dm.load(rc->get_stream());
				}
				pixels material(320, 200);
				rc->get_stream().read(reinterpret_cast <char*>(material.data()), material.get_size());
				if (rc->where_palette() == raw_image_resource::PALETTE_AT_END) {
					pal = dm.load(rc->get_stream());
				}
				return create_surface(material, pal.empty() ? load_standard_vga_palette() : pal);
			}
		}
		RAISE_EX("Should not be here");
	}
}
