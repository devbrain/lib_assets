//
// Created by igor on 6/28/24.
//

#ifndef  WINDOWS_RESOURCES_LOADER_HH
#define  WINDOWS_RESOURCES_LOADER_HH

#include <assets/resources/abstract_resource_loader.hh>
#include <assets/resources/exe/windows_resources.hh>

namespace neutrino::assets {
	class windows_resources_loader : public abstract_resource_loader<windows_resource_directory> {
		bool accept(std::istream& is) const override;
		windows_resource_directory load(std::istream& is) const override;
	};
}



#endif
