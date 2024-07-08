//
// Created by igor on 7/7/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_DETAIL_BITSTREAM_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_DETAIL_BITSTREAM_HH

#include <iosfwd>
#include <cstdint>
#include <assets/assets_export.h>

namespace neutrino::assets::detail {
	class ASSETS_EXPORT bitstream {
		public:
			explicit bitstream(std::istream& is, bool little_endian = true);
			int read(unsigned int bits, unsigned int *out);
			void flush();
		private:
			std::istream& m_stream;
			bool m_little_endian;
			uint8_t m_curr_bit;
			uint8_t m_curr_byte;
	};
}

#endif
