//
// Created by igor on 5/30/24.
//
#include <doctest/doctest.h>
#include <bsw/io/memory_stream_buf.hh>
#include <bsw/digest/md5.hh>
#include <bsw/macros.hh>
#include <assets/resources/image/image_data_manager.hh>

#include "image/data/acu100_1_gif.h"
#include "image/data/necco_gif.h"
#include "image/data/rgb1_gif.h"
#include "image/data/rgb4_gif.h"
#include "image/data/rgb6_gif.h"
#include "image/data/rgb8_gif.h"

#include "image/data/logo_jpg.h"

#include "image/data/simple_b_bh10_0_psd.h"
#include "image/data/simple_b_bh25_0_psd.h"
#include "image/data/rgb24_psd.h"
#include "image/data/rgb32rle_tga.h"

static constexpr auto digest_acu100_1_gif = "8c37e94d16073a32af40d539d534d94d";
static constexpr auto digest_necco_gif = "c2c718d99284b66052c9570e9ba9792c";
static constexpr auto digest_rgb1_gif = "32081a63b5896ebb49fdce1da439398f";
static constexpr auto digest_rgb4_gif = "bd8533531a775bdfa9d19d97459f0c12";
static constexpr auto digest_rgb6_gif = "df49824f9258a64fc8dba71728bec5e9";
static constexpr auto digest_rgb8_gif = "e0993e47c25dd1e5fc02a51c3a149872";

static constexpr auto digest_logo_jpg = "7311b470ea951420ba2a100d97cd6d4c";

static constexpr auto digest_rgb24_psd = "ef88f6e142da946a8974079ed520b41c";
static constexpr auto digest_simple_b_bh10_0_psd = "6179dac5cb1660ec6eb05347e6bc7962";
static constexpr auto digest_simple_b_bh25_0_psd = "70566ba1428854ae6dbbf3da044d4667";

static constexpr auto digest_rgb32rle_tga = "8489e4d4080c5a64151c92662e76c6a7";

static bool test_image(const unsigned char* input, std::size_t input_size, const char* expected) {
	bsw::io::memory_input_stream stream((const char*)input, input_size);
	static assets::image_data_manager dm;
	auto image = dm.load (stream);
	bsw::md5_engine md5;
	md5.update (image->pixels, image->pitch*image->h);
	return bsw::md5_engine::digest_to_hex (md5.digest()) == expected;
}

#define TEST_IMAGE(NAME) \
	test_image (NAME, sizeof (NAME), PPCAT(digest_, NAME))

TEST_SUITE("Image Data Manager") {
	TEST_CASE("gif") {
		REQUIRE(TEST_IMAGE (acu100_1_gif));
		REQUIRE(TEST_IMAGE (necco_gif));
		REQUIRE(TEST_IMAGE (rgb1_gif));
		REQUIRE(TEST_IMAGE (rgb4_gif));
		REQUIRE(TEST_IMAGE (rgb6_gif));
		REQUIRE(TEST_IMAGE (rgb8_gif));
	}

	TEST_CASE("jpg") {
		REQUIRE(TEST_IMAGE (logo_jpg));
	}

	TEST_CASE("psd") {
		REQUIRE(TEST_IMAGE (rgb24_psd));
		REQUIRE(TEST_IMAGE (simple_b_bh10_0_psd));
		REQUIRE(TEST_IMAGE (simple_b_bh25_0_psd));
	}

	TEST_CASE("tga") {
		REQUIRE(TEST_IMAGE (rgb32rle_tga));
	}
}