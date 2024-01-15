//
// Created by igor on 1/15/24.
//

#include "physical_fs.hh"
#include "physical_file_ops.hh"
#include "physical_dir_iterator_ops.hh"

namespace neutrino::assets::datastore::physical_fs {

	void physical_fs_impl::open ([[maybe_unused]] const std::string& params) {
	}

	std::string physical_fs_impl::get_name () const {
		return "physical_fs";
	}

	std::unique_ptr<file_operations> physical_fs_impl::get_file_operations (std::string internal_path) {
		return std::make_unique<physical_file_ops> (std::move(internal_path));
	}

	std::unique_ptr<directory_iterator_operations>
	physical_fs_impl::get_directory_iterator_operations (std::string internal_path) {
		return std::make_unique<physical_dir_iterator_ops> (std::move(internal_path));
	}

}