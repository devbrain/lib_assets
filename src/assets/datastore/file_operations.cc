//
// Created by igor on 1/15/24.
//

#include <assets/datastore/file_operations.hh>

namespace neutrino::assets::datastore {
	file_operations::~file_operations() = default;

	file_operations::file_operations (std::string pth) : m_path(std::move(pth)) {

	}

	const std::string& file_operations::get_path_impl () const {
		return m_path;
	}


}