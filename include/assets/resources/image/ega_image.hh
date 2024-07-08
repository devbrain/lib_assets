//
// Created by igor on 7/7/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_EGA_IMAGE_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_EGA_IMAGE_HH

#include <tuple>
#include <array>
#include <iosfwd>

#include <assets/assets_export.h>
#include <assets/resources/image/pixels.hh>

namespace neutrino::assets {
	enum class ega_bit_purpose {
		unused = 0, ///< This entry in the vector is not present, pad the vector with this
		blank, ///< This plane is empty - skip over on read, and write zeroes
		blue0, ///< Bits: 0=blue on, 1=blue off
		blue1, ///< Bits: 1=blue on, 0=blue off
		green0, ///< Bits: 0=green on, 1=green off
		green1, ///< Bits: 1=green on, 0=green off
		red0, ///< Bits: 0=red on, 1=red off
		red1, ///< Bits: 1=red on, 0=red off
		intensity0, ///< Bits: 0=bright, 1=dark
		intensity1, ///< Bits: 1=bright, 0=dark
		hit0, ///< Bits: 0=hit, 1=miss
		hit1, ///< Bits: 1=hit, 0=miss
		opaque0, ///< Bits: 0=opaque, 1=transparent
		opaque1, ///< Bits: 1=opaque, 0=transparent
	};

	enum class mask_type : uint8_t {
		transparent = 0x01, ///< Mask: 0=opaque, 1=transparent
		touch = 0x02, ///< Mask: 0=pass, 1=hit
	};

	using ega_byte_description = std::array <ega_bit_purpose, 8>;

	ASSETS_EXPORT std::tuple <pixels, pixels> load_byteplanar_ega(std::istream& is,
	                                                              const ega_byte_description& descr, unsigned int w,
	                                                              unsigned int h);
	ASSETS_EXPORT std::tuple <pixels, pixels> load_linear_ega(std::istream& is,
	                                                          const ega_byte_description& descr, unsigned int w,
	                                                          unsigned int h);
	ASSETS_EXPORT std::tuple <pixels, pixels> load_planar_ega(std::istream& is,
	                                                          const ega_byte_description& descr, unsigned int w,
	                                                          unsigned int h);
	ASSETS_EXPORT std::tuple <pixels, pixels> load_rowplanar_ega(std::istream& is,
	                                                             const ega_byte_description& descr, unsigned int w,
	                                                             unsigned int h);
}

#endif
