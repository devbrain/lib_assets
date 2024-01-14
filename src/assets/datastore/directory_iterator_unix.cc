//
// Created by igor on 1/12/24.
//

#include <assets/datastore/directory_iterator_unix.hh>
#include <assets/datastore/path.hh>
#include <assets/datastore/file.hh>

#include <bsw/predef.h>

namespace neutrino::assets::datastore {
	DirectoryIteratorImpl::DirectoryIteratorImpl(const std::string& pth): _pDir(nullptr), _rc(1)
	{
		path p(pth);
		p.make_file ();


		_pDir = opendir(p.to_string ().c_str());

		if (!_pDir) file::handle_last_error (pth);

		next();
	}

	DirectoryIteratorImpl::~DirectoryIteratorImpl()
	{
		if (_pDir) closedir(_pDir);
	}

	const std::string& DirectoryIteratorImpl::next()
	{
		do
		{
			struct dirent* pEntry = readdir(_pDir);
			if (pEntry)
				_current = pEntry->d_name;
			else
				_current.clear();
		}
		while (_current == "." || _current == "..");
		return _current;
	}
}
