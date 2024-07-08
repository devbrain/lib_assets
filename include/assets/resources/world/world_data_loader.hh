//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_DATA_MANAGER_HH_

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include <assets/resources/world/world.hh>
#include <assets/resources/data_loader.hh>
#include <sdlpp/video/surface.hh>
#include <assets/resources/resource_from_stream.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT world_fs_resolver : public resource_from_stream {
		public:
			world_fs_resolver(const std::filesystem::path& main_tmx_file, const data_loader<sdl::surface>& img_loader);

			[[nodiscard]] std::istream& get_stream() const override;
			[[nodiscard]] const data_loader<sdl::surface>& get_image_loader () const;
			[[nodiscard]] std::string resolve(const std::string& path) const;
		private:
			std::filesystem::path m_base_path;
			const data_loader<sdl::surface>& m_img_loader;
			std::unique_ptr <std::fstream> m_stream;
	};

	ASSETS_ENABLE_ISTREAM_STATE(world_fs_resolver);


	class ASSETS_EXPORT world_data_loader : public data_loader <world, world_fs_resolver> {
		public:
			world_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_DATA_MANAGER_HH_
