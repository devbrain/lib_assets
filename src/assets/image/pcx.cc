//
// Created by igor on 5/29/24.
//

#include <istream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <assets/resources/detail/istream_pos_keeper.hh>
#include <bsw/exception.hh>
#include <bsw/io/binary_reader.hh>

#include "pcx.hh"

namespace {

	struct pcx_header {
		uint8_t Manufacturer;
		uint8_t Version;
		uint8_t Encoding;
		uint8_t BitsPerPixel;
		int16_t Xmin, Ymin, Xmax, Ymax;
		int16_t HDpi, VDpi;
		uint8_t Colormap[48];
		uint8_t Reserved;
		uint8_t NPlanes;
		int16_t BytesPerLine;
		int16_t PaletteInfo;
		int16_t HscreenSize;
		int16_t VscreenSize;
		uint8_t Filler[54];
	};

	bsw::io::binary_reader& operator>> (bsw::io::binary_reader& reader, pcx_header& hdr) {
		reader >> hdr.Manufacturer >> hdr.Version >> hdr.Encoding >> hdr.BitsPerPixel
			   >> hdr.Xmin >> hdr.Ymin >> hdr.Xmax >> hdr.Ymax >> hdr.HDpi >> hdr.VDpi;
		reader.read_raw (hdr.Colormap, sizeof (hdr.Colormap));
		reader >> hdr.Reserved >> hdr.NPlanes >> hdr.BytesPerLine >> hdr.PaletteInfo
			   >> hdr.HscreenSize >> hdr.VscreenSize;
		reader.read_raw (hdr.Filler, sizeof (hdr.Filler));
		return reader;
	}

	assets::image_info pcx_info_internal (bsw::io::binary_reader& reader, pcx_header& pcxh) {
		reader >> pcxh;
		if (reader.good ()) {
			if (pcxh.Xmax < pcxh.Xmin) {
				std::swap (pcxh.Xmax, pcxh.Xmin);
			}
			if (pcxh.Ymax < pcxh.Ymin) {
				std::swap (pcxh.Ymax, pcxh.Ymin);
			}
			unsigned int width = (pcxh.Xmax - pcxh.Xmin) + 1;
			unsigned int height = (pcxh.Ymax - pcxh.Ymin) + 1;
			neutrino::sdl::pixel_format::format format;
			if ((pcxh.BitsPerPixel == 1 && pcxh.NPlanes >= 1 && pcxh.NPlanes <= 4)
				|| (pcxh.BitsPerPixel == 8 && pcxh.NPlanes == 1)) {
				format = neutrino::sdl::pixel_format::format::INDEX8;
			} else if (pcxh.BitsPerPixel == 8 && pcxh.NPlanes == 3) {
				format = neutrino::sdl::pixel_format::format::RGB24;
			} else {
				RAISE_EX("Unsupported PCX depth");
			}
			return {width, height, neutrino::sdl::pixel_format (format)};
		}
		RAISE_EX("I/O error while loading PCX");
	}

	void read_row(uint8_t* buff, std::size_t bpl, bsw::io::binary_reader& reader, bool is_rle) {
		if (!is_rle) {
			reader.read_raw (buff, bpl);
		} else {
			int count = 0;
			uint8_t ch;
			for (std::size_t i = 0; i < bpl; i++) {
				if (count == 0) {
					reader >> ch;
					if (ch < 0xc0) {
						count = 1;
					} else {
						count = ch - 0xc0;
						reader >> ch;
					}
				}
				buff[i] = ch;
				count--;
			}
		}
	}

	void expand_row_4(const uint8_t* buff, std::size_t bpl, unsigned int width, const pcx_header& pcxh, uint8_t* row) {
		/* expand planes to 1 byte/pixel */
		const uint8_t* innerSrc = buff;

		for (int plane = 0; plane < pcxh.NPlanes; plane++) {
			int x = 0;
			for (int16_t j = 0; j < pcxh.BytesPerLine; j++) {
				auto byte = *innerSrc++;
				for (int k = 7; k >= 0; k--) {
					unsigned bit = (byte >> k) & 1;
					/* skip padding bits */
					if (j * 8 + k >= width) {
						continue;
					}
					row[x++] |= bit << plane;
				}
			}
		}
	}

	void expand_row_8(const uint8_t* buff, std::size_t bpl, unsigned int width, uint8_t* row) {
		std::memcpy (row, buff, std::min ((size_t)width, bpl));
	}

	void expand_row_24(const uint8_t* buff, std::size_t bpl, unsigned int width, std::size_t pitch, const pcx_header& pcxh, uint8_t* row) {
		auto* src = buff;
		auto* src_end = buff + bpl;
		for (int plane = 0; plane < pcxh.NPlanes; plane++) {
			auto* dst = row + plane;
			auto* dst_end = row + pitch;
			for (unsigned int x = 0; x < width; x++) {
				if ((src + x) >= src_end || dst >= dst_end) {
					RAISE_EX("decoding out of bounds (corrupt?)");
				}
				*dst = src[x];
				dst += pcxh.NPlanes;
			}
			src += pcxh.BytesPerLine;
		}
	}
}
namespace assets::detail {
	bool is_pcx (std::istream& is) {
		istream_pos_keeper keeper (is);
		pcx_header pcxh{};
		bsw::io::binary_reader reader (is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
		reader >> pcxh;
		if (reader.good ()) {
			if ((pcxh.Manufacturer == 10) && (pcxh.Version == 5) && (pcxh.Encoding == 0 || pcxh.Encoding == 1)) {
				if ((pcxh.BitsPerPixel == 1 && pcxh.NPlanes >= 1 && pcxh.NPlanes <= 4)
					|| (pcxh.BitsPerPixel == 8 && pcxh.NPlanes == 1)) {
					return true;
				} else if (pcxh.BitsPerPixel == 8 && pcxh.NPlanes == 3) {
					return true;
				}
			}
		}
		return false;
	}



	image_info pcx_info (std::istream& is) {
		istream_pos_keeper keeper (is);

		bsw::io::binary_reader reader (is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
		pcx_header pcxh{};
		return pcx_info_internal (reader, pcxh);
	}

	neutrino::sdl::surface pcx_load (std::istream& is) {
		istream_pos_keeper keeper (is);
		pcx_header pcxh{};
		bsw::io::binary_reader reader (is, bsw::io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER);
		auto info = pcx_info_internal (reader, pcxh);
		int sz = is.tellg();
		auto s = neutrino::sdl::surface (info.width, info.height, info.pixel_format);
		auto src_bits = pcxh.BitsPerPixel * pcxh.NPlanes;
		std::size_t bpl = pcxh.NPlanes * pcxh.BytesPerLine;
		std::vector<uint8_t> buff (bpl, 10);
		auto* row = static_cast<uint8_t*>(s.handle ()->pixels);
		auto pitch = s.handle ()->pitch;

		for (unsigned int y = 0; y < info.height; y++) {
			read_row (buff.data(), bpl, reader, pcxh.Encoding != 0);
			if (src_bits <= 4) {
				expand_row_4 (buff.data(), bpl, info.width, pcxh, row);
			} else if (src_bits == 8) {
				expand_row_8 (buff.data(), bpl, info.width,  row);
			} else if (src_bits == 24) {
				expand_row_24 (buff.data(), bpl, info.width, pitch, pcxh, row);
			}
			row += pitch;
		}

		if (info.pixel_format.get_format() == neutrino::sdl::pixel_format::format::INDEX8) {
			auto *colors = s.handle()->format->palette->colors;
			int nc = 1 << src_bits;
			int i;

			s.handle()->format->palette->ncolors = nc;
			if (src_bits == 8) {
				uint8_t pch;
				uint8_t colormap[768];
				do {
					reader >> pch;
					if (!is) {
						/* Couldn't find the palette, try the end of the file */
						is.seekg (768, std::ios::end);
						break;
					}
				}
				while (pch != 12);
				reader.read_raw (colormap, sizeof (colormap));
				for (i = 0; i < 256; i++ ) {
					colors[i].r = colormap[i * 3 + 0];
					colors[i].g = colormap[i * 3 + 1];
					colors[i].b = colormap[i * 3 + 2];
				}
			} else {
				for ( i = 0; i < nc; i++ ) {
					colors[i].r = pcxh.Colormap[i * 3 + 0];
					colors[i].g = pcxh.Colormap[i * 3 + 1];
					colors[i].b = pcxh.Colormap[i * 3 + 2];
				}
			}
		}

		return s;
	}
}