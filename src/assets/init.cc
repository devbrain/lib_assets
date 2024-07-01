//
// Created by igor on 6/2/24.
//
#include <assets/init.hh>
#include <sdlpp/system.hh>

#if defined(ASSETS_BUILD_FOR_TESTING)
#define DOCTEST_CONFIG_IMPLEMENT_DLL
//#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <iostream>
DOCTEST_SYMBOL_EXPORT void from_dll();   // to silence "-Wmissing-declarations" with GCC
DOCTEST_SYMBOL_EXPORT void from_dll() {
	std::cout << "ZOPA" << std::endl;
} // force the creation of a .lib file with MSVC
#endif

namespace neutrino::assets {
	initializer::initializer () {
		assets_init();
	}

	initializer::~initializer () {
		assets_done();
	}

	void assets_init() {
		SAFE_SDL_CALL(TTF_Init);
		SAFE_SDL_CALL(IMG_Init, IMG_INIT_JPG | IMG_INIT_PNG );
	}

	void assets_done() {
		TTF_Quit();
		IMG_Quit();
	}
}