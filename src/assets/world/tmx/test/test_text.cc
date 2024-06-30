//
// Created by igor on 30/07/2021.
//

#include <doctest/doctest.h>
#include "test_utils.hh"
#include "world/tmx/test/data/test3/b64zlib.h"

using namespace neutrino::assets::tmx;

TEST_CASE("test text object") {
  auto the_map = test::load_map (b64zlib, b64zlib_length);

  REQUIRE(!the_map.objects ().empty ());

  const object_layer& ol = the_map.objects ()[0];
  REQUIRE(ol.objects ().size () == 5);

  const text* t = std::get_if<text> (&ol.objects ()[4]);
  REQUIRE(t);
  REQUIRE(t->data () == "Hello World");
  REQUIRE(t->bold ());
  REQUIRE(t->italic ());
  REQUIRE(t->wrap ());
  REQUIRE(t->color () == "#ff0000");
  REQUIRE(t->id () == 5);
}