//
// Created by igor on 6/23/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_AUDIO_DATA_MANAGER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_AUDIO_DATA_MANAGER_HH

#include <sdlpp/audio/sound_effect.hh>
#include <assets/resources/data_loader.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT sound_effect_data_loader : public data_loader <sdl::sound_effect> {
		public:
			sound_effect_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_AUDIO_DATA_MANAGER_HH
