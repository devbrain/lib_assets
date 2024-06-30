//
// Created by igor on 1/10/24.
//


#include <doctest/doctest.h>

DOCTEST_SYMBOL_IMPORT void from_dll();

int main(int argc, char** argv) {

	from_dll();

	doctest::Context context(argc, argv);
	int res = context.run();

	if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
		return res;          // propagate the result of the tests

	return res;
}

