//
// Created by igor on 1/15/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_OPERATIONS_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_OPERATIONS_HH_

#include <string>
#include <assets/assets_export.h>

namespace neutrino::assets::datastore {
	class ASSETS_EXPORT directory_iterator_operations {
	 public:
		virtual ~directory_iterator_operations ();

		[[nodiscard]] virtual const std::string &get () const = 0;
		virtual const std::string &next () = 0;
	};
}

#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_OPERATIONS_HH_
