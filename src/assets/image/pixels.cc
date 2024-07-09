//
// Created by igor on 7/7/24.
//

#include <sstream>
#include <assets/resources/font/rom_font.hh>
#include <assets/resources/image/pixels.hh>

namespace neutrino::assets {
	pixels::pixels(std::size_t w, std::size_t h)
		: m_width(w), m_height(h), m_pixels(w * h, 0) {
	}

	std::size_t pixels::get_width() const {
		return m_width;
	}

	std::size_t pixels::get_height() const {
		return m_height;
	}

	const uint8_t* pixels::data() const {
		return m_pixels.data();
	}

	uint8_t* pixels::data() {
		return m_pixels.data();
	}

	std::size_t pixels::get_size() const {
		return m_pixels.size();
	}

	std::tuple<sdl::surface, std::vector<sdl::rect>> render_pixels_to_surface(const std::vector<pixels>& data,
	                                                                          const sdl::color& fg_color, const sdl::color& bg_color) {
		using dims_t = std::tuple<std::size_t, std::size_t>;
		std::map<dims_t, std::vector<std::size_t>> dims2index;
		std::size_t idx = 0;
		for (const auto& px : data) {
			dims_t dims(px.get_width(), px.get_height());
			auto itr = dims2index.find(dims);
			if (itr == dims2index.end()) {
				dims2index.insert(std::make_pair(dims, std::vector<std::size_t>(1, idx)));
			} else {
				itr->second.push_back(idx);
			}
			idx++;
		}
		// evaluate dimensions
		std::vector<sdl::surface> headers;
		std::vector<sdl::surface> numbers;
		std::size_t max_num = 0;

		constexpr std::size_t tiles_in_row = 10;

		for (const auto& [d, v] : dims2index) {
			std::ostringstream os;
			os << std::get<0>(d) << "x" << std::get<1>(d) << " " << v.size() << " blocks";
			headers.push_back(render_romfont_8x8(os.str(), fg_color, bg_color));
			max_num = std::max(max_num, v.size());
		}
		for (std::size_t i=0; i<max_num; i++) {
			std::ostringstream os;
			os << i;
			numbers.push_back(render_romfont_8x8(os.str(), fg_color, bg_color));
		}
		std::vector<sdl::rect> rects(data.size());
		std::size_t max_w = 0;
		std::size_t last_y = 0;
		idx = 0;
		int h = 0;
		for (const auto& [d, v] : dims2index) {
			max_w = std::max(max_w, static_cast <std::size_t>(headers[idx]->w));
			last_y += static_cast <std::size_t>(headers[idx]->h);
			idx++;
			std::size_t last_x = 0;
			std::size_t counter = 0;

			int w = static_cast <int>(std::get<0>(d));
			h = static_cast <int>(std::get<1>(d));
			bool new_line = false;

			for (std::size_t px_idx = 0; px_idx <v.size(); px_idx++) {
				if (counter == tiles_in_row) {
					counter = 0;
					last_x = 0;
					last_y = last_y + h + 8;
					new_line = true;
				}


				last_x += std::max(w, numbers[px_idx]->w+8);
				max_w = std::max(max_w, last_x);
				counter++;
			}
			if (!new_line) {
				last_x = 0;
				last_y = last_y + h + 8;
			}
		}
		last_y = last_y + h + 8;
		sdl::surface srf = sdl::surface::make_8bit(max_w, last_y);
		srf.set_palette(load_standard_vga_palette());
		// draw
		idx = 0;
		last_y = 0;
		for (const auto& [d, v] : dims2index) {
			const auto& hdr = headers[idx++];
			sdl::rect dst_rect(0, static_cast<int>(last_y), hdr->w, hdr->h);
			sdl::rect src_rect(0, 0, hdr->w, hdr->h);
			hdr.blit(src_rect, srf, dst_rect);

			last_y += hdr->h;
			std::size_t last_x = 0;
			std::size_t counter = 0;
			int w = static_cast <int>(std::get<0>(d));
			h = static_cast <int>(std::get<1>(d));
			bool new_line = false;
			for (std::size_t px_idx = 0; px_idx <v.size(); px_idx++) {
				if (counter == tiles_in_row) {
					counter = 0;
					last_x = 0;
					last_y = last_y + h + 8;
					new_line = true;
				}
				const auto& px = data[v[px_idx]];
				for (int y=0; y<h; y++) {
					uint8_t* row = static_cast <uint8_t*>(srf->pixels)+ (y + last_y) * srf->pitch;
					for (int x=0; x<w; x++) {
						uint8_t* target = row + last_x + x;
						*target = *(px.data() + x + y*w);
					}
				}
				sdl::rect rect(static_cast <int>(last_x), static_cast <int>(last_y), w, h );
				rects[v[px_idx]] = rect;

				const auto& nm = numbers[px_idx];
				src_rect.x = 0;
				src_rect.y = 0;
				src_rect.w = nm->w;
				src_rect.h = nm->h;

				dst_rect.x = static_cast<int>(last_x);
				dst_rect.y = static_cast<int>(last_y + h);
				dst_rect.w = nm->w;
				dst_rect.h = nm->h;
				nm.blit(src_rect, srf,  dst_rect);
				last_x += std::max(w, nm->w + 8);
				counter++;
			}
			if (!new_line) {
				last_x = 0;
				last_y = last_y + h + 8;
			}
		}
		return {std::move(srf), std::move(rects)};
	}
}
