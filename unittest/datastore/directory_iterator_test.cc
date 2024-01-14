//
// Created by igor on 1/12/24.
//
#include <unistd.h>
#include <climits>
#include <stdexcept>

#include <doctest/doctest.h>
#include <assets/datastore/directory_iterator.hh>
#include <assets/datastore/path.hh>
#include <assets/datastore/file.hh>


using namespace neutrino::assets::datastore;

static void create_file (const path& p, const std::string& suffix) {
	file f1(p.to_string () + suffix);
	f1.create_file ();
}

static void create_subdir (const path& p) {
	file d(p);
	d.create_directories ();
	create_file (p, "d");
	create_file (p, "1");
	create_file (p, "A");
	create_file (p, "2");
	create_file (p, "B");
	create_file (p, "c");
}

static path get_path() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd))) {
		path p(cwd);

		p.make_directory().push_directory ("zopa");
		return p;
	}
	throw std::runtime_error("failed to get path");
}

static void setup() {
	file d(get_path());
	if (d.exists()) {
		d.remove(true);
	}
	d.create_directories();
	path p = get_path();
	create_subdir (p);

	p.push_directory("first");
	create_subdir (p);

	p.push_directory("second");
	create_subdir (p);
}

static void tear_down() {
	file d(get_path());
	if (d.exists()) {
		d.remove(true);
	}
}


TEST_CASE("directory_iterator_test") {
	setup();
	SUBCASE("test 1") {
		path p = get_path();
		directory_iterator dirIterator(p);
		directory_iterator end;
		std::vector<std::string> result;
		std::string file;

		while (dirIterator != end)
		{
			file = dirIterator->get_path();
			++dirIterator;
			result.push_back(file);
		}

		REQUIRE(7l == (long) result.size());
	}
	SUBCASE("test 2") {
		path p = get_path();
		sorted_directory_iterator dirIterator(p);
		sorted_directory_iterator end;
		std::vector<std::string> result;
		std::string file;

		while (dirIterator != end)
		{
			file = path(dirIterator->get_path()).get_file_name();
			++dirIterator;
			result.push_back(file);
		}

		REQUIRE(7 == (long) result.size());
		REQUIRE("first" == result[0]);
		REQUIRE("1" == result[1]);
		REQUIRE("2" == result[2]);
		REQUIRE("A" == result[3]);
		REQUIRE("B" == result[4]);
		REQUIRE("c" == result[5]);
		REQUIRE("d" == result[6]);
	}

	SUBCASE("test 3") {
		path p = get_path();
		simple_recursive_directory_iterator dirIterator(p);
		simple_recursive_directory_iterator end;
		std::vector<std::string> result;
		std::string file;

		while (dirIterator != end)
		{
			file = dirIterator->get_path();
			++dirIterator;
			result.push_back(file);
		}

		REQUIRE(20 == (long) result.size());
	}
	SUBCASE("test 4") {
		path p = get_path();
		siblings_first_recursive_directory_iterator dirIterator(p);
		siblings_first_recursive_directory_iterator end;
		std::vector<std::string> result;
		std::string file;

		while (dirIterator != end)
		{
			file = dirIterator->get_path();
			++dirIterator;
			result.push_back(file);
		}

		REQUIRE(20 == (long) result.size());
	}
	tear_down();
}