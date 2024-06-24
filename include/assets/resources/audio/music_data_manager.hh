//
// Created by igor on 6/23/24.
//

#ifndef ASSETS_AUDIO_DATA_MANAGER_HH
#define ASSETS_AUDIO_DATA_MANAGER_HH

#include <sdlpp/audio/music.hh>
#include <assets/resources/data_manager.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT music_data_manager : public data_manager <sdl::music> {
		public:
			music_data_manager();
	};
}

#endif //ASSETS_AUDIO_DATA_MANAGER_HH
