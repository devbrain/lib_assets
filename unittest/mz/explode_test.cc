#include <stdexcept>
#include <doctest/doctest.h>

#include "assets/resources/exe/exe_data_manager.hh"
#include <bsw/io/memory_stream_buf.hh>
#include <bsw/digest/md5.hh>

#include "mz/data/pklite_112.cc"
#include "mz/data/pklite_120.cc"
#include "mz/data/pklite_201.cc"
#include "mz/data/pklite_E_115.cc"
#include "mz/data/pklite_100.cc"
#include "mz/data/pklite_115.cc"
#include "mz/data/pklite_150.cc"
#include "mz/data/pklite_E_112.cc"
#include "mz/data/pklite_E_120.cc"

#include "mz/data/z90.cc"
#include "mz/data/z91.cc"
#include "mz/data/z91e.cc"

#include "mz/data/dot.cc"
#include "mz/data/lex.cc"
#include "mz/data/tntega.cc"

static constexpr auto digest_pklite_112 = "e1f98f301ef8bb8710ae14469bcb2cd0";
static constexpr auto digest_pklite_115 = "13482d37794b1106a85712b5e7a1227a";
static constexpr auto digest_pklite_120 = "e1f98f301ef8bb8710ae14469bcb2cd0";
static constexpr auto digest_pklite_150 = "36ce063f2a979acc3ba887f4f3b9f735";
static constexpr auto digest_pklite_201 = "e6cf27d7818c320ce64bcb4caba7f5a4";
static constexpr auto digest_pklite_E_112 = "8a4b841106bae1f32c7ca45e9d41c016";
static constexpr auto digest_pklite_E_115 = "56dccb4b55bdd7c57f09dbb584050a51";
static constexpr auto digest_pklite_E_120 = "8a4b841106bae1f32c7ca45e9d41c016";

static constexpr auto digest_lzexe_91 = "f38e4c688fcd8f3d4f102dc5e2b8bb0f";
static constexpr auto digest_lzexe_91_E = "f38e4c688fcd8f3d4f102dc5e2b8bb0f";
static constexpr auto digest_lzexe_90 = "620d7dce66a13ec7be84b9f390078aa6";

static constexpr auto digest_knowledge_dynamics_LEX = "03703e056977944b007eb2ecccf3f1c4";
static constexpr auto digest_knowledge_dynamics_DOT = "3b1429a7224c868b4725228b1a4ffb66";
static constexpr auto digest_knowledge_dynamics_TNT = "d813b5ac3095c24c3eba559bac22a32d";

using md5_digest = bsw::md5_engine::digest_t;

static md5_digest eval_digest (const unsigned char* data, std::size_t length) {
	static assets::exe_data_manager dm;
	bsw::io::memory_input_stream stream ((const char*)data, length);
	std::vector<char> out_buff = dm.load (stream);
	bsw::md5_engine md5;
	md5.update (out_buff.data (), out_buff.size ());
	return md5.digest ();
}

static bool do_test (const unsigned char* data, std::size_t length, const char* expected) {
	auto digest = eval_digest (data, length);
	return bsw::md5_engine::digest_to_hex (digest) == expected;
}



#define PKLITE_TEST(NAME) do_test (PPCAT(data::pklite_, NAME), PPCAT(PPCAT (data::pklite_, NAME), _len), PPCAT(digest_, PPCAT (pklite_, NAME)))
#define LZEXE_TEST(NAME) do_test (PPCAT (data::z, NAME), PPCAT(PPCAT (data::z, NAME), _len), PPCAT(digest_, PPCAT (lzexe_, NAME)))
#define KD_TEST(NAME) do_test (PPCAT (data::knowledge_dynamics_, NAME), PPCAT(PPCAT (data::knowledge_dynamics_, NAME), _len), PPCAT(digest_, PPCAT (knowledge_dynamics_, NAME)))

TEST_SUITE("MZ-EXPLODE") {

	TEST_CASE("PKLITE tests") {
		REQUIRE(PKLITE_TEST (112));
		REQUIRE(PKLITE_TEST (E_112));

		REQUIRE(PKLITE_TEST (115));
		REQUIRE(PKLITE_TEST (E_115));

		REQUIRE(PKLITE_TEST (150));
		//REQUIRE(PKLITE_TEST(201));
	}

	TEST_CASE("LZEXE tests") {
		REQUIRE(LZEXE_TEST (90));
		REQUIRE(LZEXE_TEST (91));
		REQUIRE(LZEXE_TEST (91_E));
	}

	TEST_CASE("KnowledgeDynamics tests") {
		REQUIRE(KD_TEST (DOT));
		REQUIRE(KD_TEST (TNT));
		REQUIRE(KD_TEST (LEX));
	}
}