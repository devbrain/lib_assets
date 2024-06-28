//
// Created by igor on 6/28/24.
//

#include "windows_resources_loader.hh"
#include "mz/win_exe/ms_exe_file_type.hh"
#include "mz/win_exe/pe_resource_builder.hh"
#include "mz/win_exe/windows_pe_file.hh"
#include "mz/win_exe/windows_ne_file.hh"
#include "mz/win_exe/abstract_reporter.hh"
#include <bsw/exception.hh>

namespace neutrino::assets {
	bool windows_resources_loader::accept(std::istream& is) const {
		auto type = get_ms_exe_file_type(is);
		return type == NE || type == PE;
	}

	windows_resource_directory windows_resources_loader::load(std::istream& is) const {
		auto type = get_ms_exe_file_type(is);
		if (type == PE) {
			abstract_reporter reporter;
			auto f = std::make_unique<windows_pe_file>(is, reporter);
			return build_resources(std::move(f));
		} else if (type == NE) {
			auto f = std::make_unique<windows_ne_file>(is);
			return build_resources(std::move(f));
		}
		RAISE_EX("Should not be here");
	}
}