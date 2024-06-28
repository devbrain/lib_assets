//
// Created by igor on 6/28/24.
//

#ifndef  WIN_ICON_LOADER_HH
#define  WIN_ICON_LOADER_HH

#include <memory>

#include <assets/resources/abstract_resource_loader.hh>
#include <assets/resources/tileset/tileset.hh>
#include <assets/resources/exe/windows_resources.hh>

namespace neutrino::assets {
	class win_icon_loader : public abstract_resource_loader <tileset> {
		public:
			win_icon_loader();

		private:
			bool accept(std::istream& is) const override;
			tileset load(std::istream& is) const override;

			std::unique_ptr <abstract_resource_loader<windows_resource_directory>> m_exe_loader;
	};
}

#endif
