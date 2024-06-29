//
// Created by igor on 5/28/24.
//

#include <istream>
#include <cstring>
#include <bsw/exception.hh>
#include <assets/resources/detail/istream_pos_keeper.hh>
#include "stb_wrapper.hh"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_NO_JPEG
#define STBI_NO_BMP
//        STBI_NO_PSD
//        STBI_NO_TGA
#define   STBI_NO_GIF
#define   STBI_NO_HDR
#define   STBI_NO_PIC
#define   STBI_NO_PNM   //(.ppm and .pgm)
#include "thirdparty/stb_image.h"

namespace {
	int istream_read (void* user, char* data, int size) {
		auto* stream = reinterpret_cast<std::istream*>(user);
		auto pos = stream->tellg ();
		stream->read (data, size);
		return static_cast<int>(stream->tellg () - pos);
	}

	void istream_skip (void* user, int n) {
		auto* stream = reinterpret_cast<std::istream*>(user);
		if (n > 0) {
			stream->seekg (n, std::ios::cur);
		} else {
			auto pos = stream->tellg ();
			stream->seekg (static_cast<int>(pos) + n, std::ios::beg);
		}
	}

	int istream_eof (void* user) {
		auto* stream = reinterpret_cast<std::istream*>(user);
		return stream->eof () ? 1 : 0;
	}

	using header_func_t = int (*) (stbi__context* s, int* x, int* y, int* comp);
	using tester_func_t = int (*) (stbi__context* s);

	bool test_image (std::istream& is, tester_func_t tester) {
		neutrino::assets::detail::istream_pos_keeper keeper(is);
		static stbi_io_callbacks io_callbacks = {
			istream_read,
			istream_skip,
			istream_eof
		};

		stbi__context s;
		stbi__start_callbacks (&s, &io_callbacks, &is);
		return tester (&s);
	}

	neutrino::sdl::surface load_image (std::istream& is, header_func_t hdrfunc) {
		neutrino::assets::detail::istream_pos_keeper keeper(is);
		static stbi_io_callbacks io_callbacks = {
			istream_read,
			istream_skip,
			istream_eof
		};

		stbi__context s;
		stbi__start_callbacks (&s, &io_callbacks, &is);

		int x;
		int y;
		int comp;
		ENFORCE(hdrfunc (&s, &x, &y, &comp));

		keeper.rewind();

		int bppToUse = (comp == STBI_grey || comp == STBI_rgb) ? STBI_rgb : STBI_rgb_alpha;
		neutrino::sdl::pixel_format::format format = (bppToUse == STBI_rgb) ?
													 neutrino::sdl::pixel_format::RGB24 :
													 neutrino::sdl::pixel_format::RGBA32;


		void* image_data = stbi_load_from_callbacks (&io_callbacks, &is, &x, &y, &comp, bppToUse);
		ENFORCE(image_data != nullptr);
		neutrino::sdl::surface surface((unsigned)x, (unsigned)y, neutrino::sdl::pixel_format (format));
		std::memcpy(surface->pixels, image_data, y * x * bppToUse);
		STBI_FREE(image_data);
		return surface;
	}
}

namespace neutrino::assets::detail {
#ifndef STBI_NO_GIF
	bool is_gif (std::istream& is) {
		return test_image (is, stbi__gif_test);
	}


	sdl::surface gif_load (std::istream& is) {
		return load_image (is, stbi__gif_info);
	}
#endif
	bool is_tga (std::istream& is) {
		return test_image (is, stbi__tga_test);
	}

	sdl::surface tga_load (std::istream& is) {
		return load_image (is, stbi__tga_info);
	}
#ifndef STBI_NO_PNG
	bool is_png (std::istream& is) {
		return test_image (is, stbi__png_test);
	}


	sdl::surface png_load (std::istream& is) {
		return load_image (is, stbi__png_info);
	}
#endif

#ifndef STBI_NO_JPEG
	bool is_jpeg (std::istream& is) {
		return test_image (is, stbi__jpeg_test);
	}


	sdl::surface jpeg_load (std::istream& is) {
		return load_image (is, stbi__jpeg_info);
	}
#endif

	bool is_psd (std::istream& is) {
		return test_image (is, stbi__psd_test);
	}

	sdl::surface psd_load (std::istream& is) {
		return load_image (is, stbi__psd_info);
	}
}