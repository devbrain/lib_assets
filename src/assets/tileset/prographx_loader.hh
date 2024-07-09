//
// Created by igor on 7/7/24.
//

#ifndef  PROGRAPHX_LOADER_HH
#define  PROGRAPHX_LOADER_HH

#include <assets/resources/abstract_resource_loader.hh>
#include <assets/resources/tileset/tileset_data_loader.hh>
#include <assets/resources/tileset/tileset.hh>

namespace neutrino::assets {
	class prographx_loader : public abstract_resource_loader <tileset, tileset_resource> {
		[[nodiscard]] bool accept(const tileset_resource& arg) const override;
		[[nodiscard]] tileset load(const tileset_resource& arg) const override;
	};
}



#endif
