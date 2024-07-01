//
// Created by igor on 6/23/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_AUDIO_DATA_LOADER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_AUDIO_DATA_LOADER_HH

#include <sdlpp/audio/music.hh>
#include <assets/resources/data_loader.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT music_data_loader : public data_loader <sdl::music> {
		public:
			music_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_AUDIO_DATA_LOADER_HH
