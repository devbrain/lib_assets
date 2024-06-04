//
// Created by igor on 6/2/24.
//
#include <assets/init.hh>
#include <sdlpp/system.hh>


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