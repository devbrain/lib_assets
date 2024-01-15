//
// Created by igor on 1/15/24.
//

#ifndef ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_DIR_ITERATOR_OPS_HH_
#define ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_DIR_ITERATOR_OPS_HH_

#include <assets/datastore/directory_iterator_operations.hh>
#include <bsw/predef.h>

#if defined(PREDEF_PLATFORM_UNIX)
#include <dirent.h>
#else
#if defined(_WIN32)
	#if !defined(WIN32_LEAN_AND_MEAN)
			#define WIN32_LEAN_AND_MEAN
	#endif
#endif
#include <windows.h>
#endif

namespace neutrino::assets::datastore::physical_fs {
	class physical_dir_iterator_ops : public directory_iterator_operations {
	 public:
		explicit physical_dir_iterator_ops(std::string internal_path);
		~physical_dir_iterator_ops() override;
		[[nodiscard]] const std::string &get () const;
		const std::string &next ();
	 private:
#if defined(PREDEF_PLATFORM_UNIX)
		std::string _current;
		DIR*        _pDir;
		int _rc;
#else
		std::string      _current;
		HANDLE           _fh;
		WIN32_FIND_DATAW _fd;
		int _rc;
#endif
		const std::string& _next ();
	};
}

#endif //ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_DIR_ITERATOR_OPS_HH_
