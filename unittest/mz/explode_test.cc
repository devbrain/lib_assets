#include <iostream>
#include <sstream>
#include <stdexcept>
#include <doctest/doctest.h>

#include <assets/mz/explode_exe_file.hh>

#include "utils/md5.h"

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



static const char* digest_pklite_112   = "e1f98f301ef8bb8710ae14469bcb2cd0";
static const char* digest_pklite_115   = "13482d37794b1106a85712b5e7a1227a";
static const char* digest_pklite_120   = "e1f98f301ef8bb8710ae14469bcb2cd0";
static const char* digest_pklite_150   = "36ce063f2a979acc3ba887f4f3b9f735";
static const char* digest_pklite_201   = "e6cf27d7818c320ce64bcb4caba7f5a4";
static const char* digest_pklite_E_112 = "8a4b841106bae1f32c7ca45e9d41c016";
static const char* digest_pklite_E_115 = "56dccb4b55bdd7c57f09dbb584050a51";
static const char* digest_pklite_E_120 = "8a4b841106bae1f32c7ca45e9d41c016";

static const char* digest_lzexe_91   = "f38e4c688fcd8f3d4f102dc5e2b8bb0f"; 
static const char* digest_lzexe_91_E = "f38e4c688fcd8f3d4f102dc5e2b8bb0f";
static const char* digest_lzexe_90   = "620d7dce66a13ec7be84b9f390078aa6";


static const char* digest_knowledge_dynamics_LEX = "03703e056977944b007eb2ecccf3f1c4";
static const char* digest_knowledge_dynamics_DOT = "3b1429a7224c868b4725228b1a4ffb66";
static const char* digest_knowledge_dynamics_TNT = "d813b5ac3095c24c3eba559bac22a32d";

typedef unsigned char md5_digest[MD5_DIGEST_LENGTH];


static void eval_digest(const unsigned char* data, std::size_t length, md5_digest& digest)
{
	std::vector<char> out_buff;
	REQUIRE(assets::mz::explode_exe_file((char*)data, length, out_buff));

	MD5_CTX c;
	MD5_Init(&c);
	MD5_Update(&c, &out_buff[0], static_cast <unsigned long> (out_buff.size ()));
	MD5_Final(digest, &c);
}
static bool do_test(const unsigned char* data, std::size_t length, const char* expected)
{
	md5_digest dgst = {0};
	bool ok = true;

		eval_digest (data, length, dgst);

		for (int n = 0; n < MD5_DIGEST_LENGTH; n++)
		{
			std::string h(expected + 2 * n, expected + 2 * (n + 1));
			std::istringstream is(h);
			int x;
			is >> std::hex >> x;
			if (x != dgst[n])
			{
				ok = false;
				break;
			}
		}
	return ok;
}

#define CONCATENATE_DIRECT(s1,s2) s1##s2
#define CONCATENATE(s1,s2) CONCATENATE_DIRECT(s1,s2)

#define STRINGIZE_HELPER(exp) #exp
#define STRINGIZE(exp) STRINGIZE_HELPER(exp)

#define PKLITE_TEST(NAME) do_test (CONCATENATE (data::pklite_, NAME), CONCATENATE(CONCATENATE (data::pklite_, NAME), _len), CONCATENATE(digest_, CONCATENATE (pklite_, NAME)))
#define LZEXE_TEST(NAME) do_test (CONCATENATE (data::z, NAME), CONCATENATE(CONCATENATE (data::z, NAME), _len), CONCATENATE(digest_, CONCATENATE (lzexe_, NAME)))
#define KD_TEST(NAME) do_test (CONCATENATE (data::knowledge_dynamics_, NAME), CONCATENATE(CONCATENATE (data::knowledge_dynamics_, NAME), _len), CONCATENATE(digest_, CONCATENATE (knowledge_dynamics_, NAME)))

TEST_SUITE("MZ-EXPLODE") {

	TEST_CASE("mz-explode PKLITE tests") {
		REQUIRE(PKLITE_TEST (112));
		REQUIRE(PKLITE_TEST (E_112));

		REQUIRE(PKLITE_TEST (115));
		REQUIRE(PKLITE_TEST (E_115));

		REQUIRE(PKLITE_TEST (150));
		//REQUIRE(PKLITE_TEST(201));
	}

	TEST_CASE("mz-explode LZEXE tests") {
		REQUIRE(LZEXE_TEST (90));
		REQUIRE(LZEXE_TEST (91));
		REQUIRE(LZEXE_TEST (91_E));
	}

	TEST_CASE("mz-explode KnowledgeDynamics tests") {
		REQUIRE(KD_TEST (DOT));
		REQUIRE(KD_TEST (TNT));
		REQUIRE(KD_TEST (LEX));
	}

}