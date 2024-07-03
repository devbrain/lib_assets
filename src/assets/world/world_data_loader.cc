//
// Created by igor on 6/23/24.
//

#include <string>
#include <fstream>
#include <streambuf>
#include <assets/resources/world/world_data_loader.hh>
#include <sdlpp/io/rwops_stream.hh>
#include <bsw/exception.hh>
#include "world/tmx_loader.hh"

namespace neutrino::assets {
	class world_loader : public abstract_resource_loader <world, world_fs_resolver> {
		bool accept([[maybe_unused]] std::istream& is, [[maybe_unused]] const world_fs_resolver& arg) const override {
			return true;
		};

		world load(std::istream& is, const world_fs_resolver& arg) const override {
			sdl::rw_istream rwops(is);
			return tmx::load(is, [&arg](const std::string& p) {
				return arg.resolve(p);
			}, arg.get_image_loader());
		}
	};

	world_fs_resolver::world_fs_resolver(const std::filesystem::path& main_tmx_file,
	                                     const data_loader <sdl::surface>& img_loader)
		: m_base_path(main_tmx_file.parent_path()),
		  m_img_loader(img_loader),
		  m_stream(std::make_unique <std::fstream>(main_tmx_file)) {
		if (!m_stream) {
			RAISE_EX("Failed to open TMX file ", main_tmx_file.u8string());
		}
	}

	std::istream& world_fs_resolver::get_stream() const {
		return *m_stream;
	}

	const data_loader<sdl::surface>& world_fs_resolver::get_image_loader() const {
		return m_img_loader;
	}

	std::string world_fs_resolver::resolve(const std::string& path) const {
		std::ifstream t(m_base_path / path);
		return {
			(std::istreambuf_iterator <char>(t)),
			std::istreambuf_iterator <char>()
		};
	}

	world_data_loader::world_data_loader() {
		register_loader("TMX", std::make_unique <world_loader>());
	}
}
