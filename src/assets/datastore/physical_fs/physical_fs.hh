//
// Created by igor on 1/15/24.
//

#ifndef ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_FS_HH_
#define ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_FS_HH_

#include <assets/datastore/filesystem.hh>

namespace neutrino::assets::datastore::physical_fs {
	class physical_fs_impl : public filesystem {
	 public:
		void open(const std::string &params) override;
		std::string get_name() const override;

		std::unique_ptr<file_operations> get_file_operations(std::string internal_path);
		std::unique_ptr<directory_iterator_operations> get_directory_iterator_operations(std::string internal_path) override;
	};
}

#endif //ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_FS_HH_
