//
// Created by igor on 1/15/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_FILESYSTEM_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_FILESYSTEM_HH_

#include <memory>
#include <string>
#include <assets/assets_export.h>

namespace neutrino::assets::datastore {
	class file_operations;
	class directory_iterator_operations;

	class ASSETS_EXPORT filesystem {
	 public:
		virtual ~filesystem();

		virtual void open(const std::string &params) = 0;
		[[nodiscard]] virtual std::string get_name() const = 0;

		[[nodiscard]] virtual std::unique_ptr<file_operations> get_file_operations(std::string internal_path) = 0;
		[[nodiscard]] virtual std::unique_ptr<directory_iterator_operations> get_directory_iterator_operations(std::string internal_path) = 0;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_FILESYSTEM_HH_
