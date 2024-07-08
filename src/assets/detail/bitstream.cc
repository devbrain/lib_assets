//
// Created by igor on 7/7/24.
//

#include <istream>
#include <assets/resources/detail/bitstream.hh>

namespace neutrino::assets::detail {
	bitstream::bitstream(std::istream& is, bool little_endian)
		: m_stream(is), m_little_endian(little_endian), m_curr_bit(8), m_curr_byte(0) {
	}

	int bitstream::read(unsigned int bits, unsigned int* out) {
		if (m_curr_bit == 8) {
			flush();
		}

		int bits_read = 0;
		while (bits > 0) {
			if (m_curr_bit == 8) {
				flush();
				if (m_stream.eof()) {
					return bits_read;
				}
			}

			unsigned int bufBitsRemaining = 8 - m_curr_bit;
			unsigned int bitsNow = (bits > bufBitsRemaining) ? bufBitsRemaining : bits;

			// Figure out which bits in the buffered byte we're interested in
			int exval;
			if (m_little_endian) {
				uint8_t mask = ~(0xFF << bitsNow);
				exval = (m_curr_byte >> m_curr_bit) & mask;
			} else {
				uint8_t mask = ~(0xFF >> bitsNow);
				exval = (m_curr_byte << m_curr_bit) & mask;
				exval >>= 8 - bitsNow;
			}

			// Work out where they have to go in the current number
			if (m_little_endian) {
				// Shift the exval up to the top of the number.
				*out |= exval << bits_read;
			} else {
				// Shift the previously-read value up out of the way and write exval
				// into the (now empty) lower bits.
				*out <<= bitsNow;
				*out |= exval;
			}

			bits -= bitsNow;
			bits_read += bitsNow;
			m_curr_bit += bitsNow;
		}
		return bits_read;
	}

	void bitstream::flush() {
		m_stream.read(reinterpret_cast <char*>(&m_curr_byte), 1);
		m_curr_bit = 0;
	}
}
