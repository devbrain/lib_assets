//
// Created by igor on 1/12/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_UNIX_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_UNIX_HH_

#include <assets/assets_export.h>
#include <string>
#include <dirent.h>

namespace neutrino::assets::datastore {
	class ASSETS_EXPORT DirectoryIteratorImpl {
	 public:
		explicit DirectoryIteratorImpl (const std::string &path);
		~DirectoryIteratorImpl ();

		void duplicate ();
		void release ();

		[[nodiscard]] const std::string &get () const;
		const std::string &next ();

	 private:
		DIR *_pDir;
		std::string _current;
		int _rc;
	};

//
// inlines
//
	inline const std::string &DirectoryIteratorImpl::get () const {
		return _current;
	}

	inline void DirectoryIteratorImpl::duplicate () {
		++_rc;
	}

	inline void DirectoryIteratorImpl::release () {
		if (--_rc == 0) {
			delete this;
		}
	}
}

#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_UNIX_HH_
