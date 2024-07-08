//
// Created by igor on 7/7/24.
//

#include <istream>
#include <assets/resources/image/ega_image.hh>
#include <assets/resources/detail/bitstream.hh>

namespace neutrino::assets {
	std::tuple <pixels, pixels> load_byteplanar_ega(std::istream& is,
	                                                const ega_byte_description& descr, unsigned int w,
	                                                unsigned int h) {
		pixels img(w, h);
		pixels mask(w, h);

		auto img_data = img.data();
		auto mask_data = mask.data();
		for (unsigned int y = 0; y < h; y++) {
			for (unsigned int x = 0; x < w; x += 8) {
				for (const auto p : descr) {
					if (p == ega_bit_purpose::unused) break;

					bool do_mask = false, swap = false;
					uint8_t value = 0;
					switch (p) {
						case ega_bit_purpose::unused: continue;
						case ega_bit_purpose::blank:
							do_mask = false;
							value = 0x00;
							swap = false;
							break;
						case ega_bit_purpose::blue0:
							do_mask = false;
							value = 0x01;
							swap = true;
							break;
						case ega_bit_purpose::blue1:
							do_mask = false;
							value = 0x01;
							swap = false;
							break;
						case ega_bit_purpose::green0:
							do_mask = false;
							value = 0x02;
							swap = true;
							break;
						case ega_bit_purpose::green1:
							do_mask = false;
							value = 0x02;
							swap = false;
							break;
						case ega_bit_purpose::red0:
							do_mask = false;
							value = 0x04;
							swap = true;
							break;
						case ega_bit_purpose::red1:
							do_mask = false;
							value = 0x04;
							swap = false;
							break;
						case ega_bit_purpose::intensity0:
							do_mask = false;
							value = 0x08;
							swap = true;
							break;
						case ega_bit_purpose::intensity1:
							do_mask = false;
							value = 0x08;
							swap = false;
							break;
						case ega_bit_purpose::hit0:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::touch);
							swap = true;
							break;
						case ega_bit_purpose::hit1:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::touch);
							swap = false;
							break;
						case ega_bit_purpose::opaque0:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::transparent);
							swap = false;
							break;
						case ega_bit_purpose::opaque1:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::transparent);
							swap = true;
							break;
					}

					uint8_t next_byte;
					is.read(reinterpret_cast <char*>(&next_byte), 1);

					// See which bit we should read down to (starting at 7.)  This is only
					// used when the image is not an even multiple of 8.
					int bits = 0;
					if (x + 8 > w) {
						bits = 8 - (w % 8);
					}

					// Run through all the (valid) bits in this byte
					auto row_data = do_mask ? mask_data : img_data;
					for (int b = 7; b >= bits; b--) {
						*row_data++ |= (((next_byte >> b) & 1) ^ swap) ? value : 0x00;
					}
				}
				int len_cell = 8;
				if (x + 8 > w) {
					len_cell = w - x;
				}
				img_data += len_cell;
				mask_data += len_cell;
			}
		}
		return {std::move(img), std::move(mask)};
	}

	std::tuple <pixels, pixels> load_linear_ega(std::istream& is,
	                                            const ega_byte_description& descr, unsigned int w,
	                                            unsigned int h) {
		pixels img(w, h);
		pixels mask(w, h);

		auto img_data = img.data();
		auto mask_data = mask.data();

		detail::bitstream bits(is);

		for (unsigned int y = 0; y < h; y++) {
			for (unsigned int x = 0; x < w; x++) {
				for (const auto p : descr) {
					if (p == ega_bit_purpose::unused) {
						break;
					}

					unsigned int bit;
					bits.read(1, &bit);
					bool do_mask = false, swap = false;
					uint8_t value = 0;
					switch (p) {
						case ega_bit_purpose::unused: continue;
						case ega_bit_purpose::blank:
							do_mask = false;
							value = 0x00;
							swap = false;
							break;
						case ega_bit_purpose::blue0:
							do_mask = false;
							value = 0x01;
							swap = true;
							break;
						case ega_bit_purpose::blue1:
							do_mask = false;
							value = 0x01;
							swap = false;
							break;
						case ega_bit_purpose::green0:
							do_mask = false;
							value = 0x02;
							swap = true;
							break;
						case ega_bit_purpose::green1:
							do_mask = false;
							value = 0x02;
							swap = false;
							break;
						case ega_bit_purpose::red0:
							do_mask = false;
							value = 0x04;
							swap = true;
							break;
						case ega_bit_purpose::red1:
							do_mask = false;
							value = 0x04;
							swap = false;
							break;
						case ega_bit_purpose::intensity0:
							do_mask = false;
							value = 0x08;
							swap = true;
							break;
						case ega_bit_purpose::intensity1:
							do_mask = false;
							value = 0x08;
							swap = false;
							break;
						case ega_bit_purpose::hit0:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::touch);
							swap = true;
							break;
						case ega_bit_purpose::hit1:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::touch);
							swap = false;
							break;
						case ega_bit_purpose::opaque0:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::transparent);
							swap = false;
							break;
						case ega_bit_purpose::opaque1:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::transparent);
							swap = true;
							break;
					}

					auto row_data = do_mask ? mask_data : img_data;

					if (swap) bit ^= 1;

					if (bit) *row_data |= value;
				}
				img_data++;
				mask_data++;
			}
			// Always start each row on a byte boundary
			bits.flush();
		}

		return {std::move(img), std::move(mask)};
	}

	std::tuple <pixels, pixels> load_planar_ega(std::istream& is,
	                                            const ega_byte_description& descr, unsigned int w,
	                                            unsigned int h) {
		pixels img(w, h);
		pixels mask(w, h);

		auto img_data = img.data();
		auto mask_data = mask.data();

		unsigned int plane_size_bytes = h * ((w + 7) / 8);
		std::streamoff len_skip = 0;
		for (const auto p : descr) {
			if (p == ega_bit_purpose::unused) continue;
			if (p == ega_bit_purpose::blank) {
				// Don't waste time processing a plane we're ignoring
				len_skip += plane_size_bytes;
				// Don't do the skip here in case the unused planes are all at the end
				// and we will end up seeking past EOF
				continue;
			}
			if (len_skip) {
				// We're processing this plane and it's following one or more ignored
				// planes, so now do the seek.
				is.seekg(len_skip, std::ios::cur);
			}

			bool do_mask = false, swap = false;
			uint8_t value = 0;
			switch (p) {
				case ega_bit_purpose::unused: continue;
				case ega_bit_purpose::blank: continue;
				case ega_bit_purpose::blue0:
					do_mask = false;
					value = 0x01;
					swap = true;
					break;
				case ega_bit_purpose::blue1:
					do_mask = false;
					value = 0x01;
					swap = false;
					break;
				case ega_bit_purpose::green0:
					do_mask = false;
					value = 0x02;
					swap = true;
					break;
				case ega_bit_purpose::green1:
					do_mask = false;
					value = 0x02;
					swap = false;
					break;
				case ega_bit_purpose::red0:
					do_mask = false;
					value = 0x04;
					swap = true;
					break;
				case ega_bit_purpose::red1:
					do_mask = false;
					value = 0x04;
					swap = false;
					break;
				case ega_bit_purpose::intensity0:
					do_mask = false;
					value = 0x08;
					swap = true;
					break;
				case ega_bit_purpose::intensity1:
					do_mask = false;
					value = 0x08;
					swap = false;
					break;
				case ega_bit_purpose::hit0:
					do_mask = true;
					value = static_cast <uint8_t>(mask_type::touch);
					swap = true;
					break;
				case ega_bit_purpose::hit1:
					do_mask = true;
					value = static_cast <uint8_t>(mask_type::touch);
					swap = false;
					break;
				case ega_bit_purpose::opaque0:
					do_mask = true;
					value = static_cast <uint8_t>(mask_type::transparent);
					swap = false;
					break;
				case ega_bit_purpose::opaque1:
					do_mask = true;
					value = static_cast <uint8_t>(mask_type::transparent);
					swap = true;
					break;
			}

			auto row_data = do_mask ? mask_data : img_data;
			for (unsigned int y = 0; y < h; y++) {
				// Run through each lot of eight pixels (a "cell"), including a partial
				// cell at the end if the width isn't a multiple of 8.
				for (unsigned int x = 0; x < w; x += 8) {
					uint8_t next_byte;
					is.read(reinterpret_cast <char*>(&next_byte), 1);

					// See which bit we should read down to (starting at 7.)  This is only
					// used when the image is not an even multiple of 8.
					int bits = 0;
					if (x + 8 > w) {
						bits = 8 - (w % 8);
					}

					// Run through all the (valid) bits in this byte
					for (int b = 7; b >= bits; b--) {
						*row_data++ |= (((next_byte >> b) & 1) ^ swap) ? value : 0x00;
					}
				}
			}
		}
		return {std::move(img), std::move(mask)};
	}

	std::tuple <pixels, pixels> load_rowplanar_ega(std::istream& is,
	                                               const ega_byte_description& descr, unsigned int w,
	                                               unsigned int h) {
		pixels img(w, h);
		pixels mask(w, h);

		for (unsigned int y = 0; y < h; y++) {
			for (const auto p : descr) {
				if (p == ega_bit_purpose::unused) break;

				auto img_data = img.data() + y * w;
				auto mask_data = mask.data() + y * w;

				// Run through each lot of eight pixels (a "cell"), including a partial
				// cell at the end if the width isn't a multiple of 8.
				for (unsigned int x = 0; x < w; x += 8) {
					bool do_mask = false, swap = false;
					uint8_t value = 0;
					switch (p) {
						case ega_bit_purpose::unused: continue;
						case ega_bit_purpose::blank:
							do_mask = false;
							value = 0x00;
							swap = false;
							break;
						case ega_bit_purpose::blue0:
							do_mask = false;
							value = 0x01;
							swap = true;
							break;
						case ega_bit_purpose::blue1:
							do_mask = false;
							value = 0x01;
							swap = false;
							break;
						case ega_bit_purpose::green0:
							do_mask = false;
							value = 0x02;
							swap = true;
							break;
						case ega_bit_purpose::green1:
							do_mask = false;
							value = 0x02;
							swap = false;
							break;
						case ega_bit_purpose::red0:
							do_mask = false;
							value = 0x04;
							swap = true;
							break;
						case ega_bit_purpose::red1:
							do_mask = false;
							value = 0x04;
							swap = false;
							break;
						case ega_bit_purpose::intensity0:
							do_mask = false;
							value = 0x08;
							swap = true;
							break;
						case ega_bit_purpose::intensity1:
							do_mask = false;
							value = 0x08;
							swap = false;
							break;
						case ega_bit_purpose::hit0:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::touch);
							swap = true;
							break;
						case ega_bit_purpose::hit1:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::touch);
							swap = false;
							break;
						case ega_bit_purpose::opaque0:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::transparent);
							swap = false;
							break;
						case ega_bit_purpose::opaque1:
							do_mask = true;
							value = static_cast <uint8_t>(mask_type::transparent);
							swap = true;
							break;
					}

					uint8_t next_byte;
					is.read(reinterpret_cast <char*>(&next_byte), 1);

					// See which bit we should read down to (starting at 7.)  This is only
					// used when the image is not an even multiple of 8.
					int bits = 0;
					if (x + 8 > w) {
						bits = 8 - (w % 8);
					}

					// Run through all the (valid) bits in this byte
					auto row_data = do_mask ? mask_data : img_data;
					for (int b = 7; b >= bits; b--) {
						*row_data++ |= (((next_byte >> b) & 1) ^ swap) ? value : 0x00;
					}
					img_data += 8;
					mask_data += 8;
				}
			}
		}
		return {std::move(img), std::move(mask)};
	}
}
