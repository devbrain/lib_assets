//
// Created by igor on 7/2/24.
//

#include <vector>
#include <assets/resources/world/world.hh>

#include <assets/resources/world/builder/texture_atlas_builder.hh>

#include <bsw/override.hh>
#include <bsw/exception.hh>
#include <bsw/logger/logger.hh>
#include <bsw/io/memory_stream_buf.hh>
#include <bsw/mp/type_name/type_name.hpp>
#include <utility>

#include "tmx/xml_reader.hh"
#include "tmx/json_reader.hh"
#include "tmx/color.hh"
#include "tmx/map.hh"

#include "tmx_loader.hh"

#include "assets/resources/world/world_image_layer.hh"

namespace neutrino::assets::tmx {
	static map load_map(const char* data, std::size_t length, path_resolver_t resolver) {
		const auto doc_type = reader::guess_document_type(data, length);
		switch (doc_type) {
			case reader::XML_DOCUMENT:
				return map::parse(xml_reader::load(data, length, "map"), std::move(resolver));
			case reader::JSON_DOCUMENT:
				return map::parse(json_reader::load(data, length, nullptr), std::move(resolver));
			default:
				RAISE_EX("Unknown document type");
		}
	}

	static sdl::surface create_surface(std::istream& is, const image& img,
	                                   const data_loader <sdl::surface>& image_loader) {
		auto s = image_loader.load(is);
		if (img.transparent()) {
			colori c(*img.transparent());
			s.color_key(sdl::color(c.r, c.g, c.b, c.a));
		}
		return s;
	}

	static sdl::surface create_surface(const image& img, const path_resolver_t& resolver,
	                                   const data_loader <sdl::surface>& image_loader) {
		if (!img.data().empty()) {
			bsw::io::memory_input_stream is(img.data().data(), img.data().size());
			return create_surface(is, img, image_loader);
		}
		const auto content = resolver(img.source());
		bsw::io::memory_input_stream is(content.c_str(), content.size());
		return create_surface(is, img, image_loader);
	}

	static void load_images(world& out, const map& in, const path_resolver_t& resolver,
	                        const data_loader <sdl::surface>& image_loader) {
		for (const auto& layer : in.layers()) {
			std::visit(bsw::overload(
				           [&out, &resolver, &image_loader](const image_layer& l) {
					           if (const image* img = l.get_image()) {
						           image_id_t image_id(std::hash <image>{}(*img));
						           out.add_image(image_id, create_surface(*img, resolver, image_loader));
					           }
				           },
				           [](const auto&) {
				           }
			           ), layer);
		}
		for (const auto& ts : in.tile_sets()) {
			if (const image* img = ts.get_image()) {
				image_id_t image_id(std::hash <image>{}(*img));
				out.add_image(image_id, create_surface(*img, resolver, image_loader));
			}

			for (const auto& t : ts) {
				if (const image* img = t.get_image()) {
					image_id_t image_id(std::hash <image>{}(*img));
					out.add_image(image_id, create_surface(*img, resolver, image_loader));
				}
			}
		}
	}

	static world load(const char* text, std::size_t size, const path_resolver_t& resolver,
	                  const data_loader <sdl::surface>& image_loader) {
		const auto raw = load_map(text, size, resolver);
		auto icolor = raw.background_color();

		world w(raw.orientation(),
		        raw.render_order(),
		        sdl::area_type{static_cast <int>(raw.width()), static_cast <int>(raw.height())},
		        sdl::area_type{static_cast <int>(raw.tile_width()), static_cast <int>(raw.tile_height())},
		        sdl::color{icolor.r, icolor.g, icolor.b, icolor.a},
		        raw.hex_side_length(),
		        raw.stagger_axis(),
		        raw.stagger_index(),
		        raw.infinite());

		load_images(w, raw, resolver, image_loader);

		return w;
	}

	world load(std::istream& is, const path_resolver_t& resolver, const data_loader <sdl::surface>& image_loader) {
		auto txt = std::vector <char>((std::istreambuf_iterator <char>(is)),
		                              std::istreambuf_iterator <char>());
		return load(txt.data(), txt.size(), resolver, image_loader);
	}
}
