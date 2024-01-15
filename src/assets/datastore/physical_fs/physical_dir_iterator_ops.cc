//
// Created by igor on 1/15/24.
//

#include <utility>

#include "physical_dir_iterator_ops.hh"
#include <assets/datastore/path.hh>
#include <assets/datastore/file.hh>

namespace neutrino::assets::datastore::physical_fs {
	physical_dir_iterator_ops::physical_dir_iterator_ops (std::string internal_path)
	: _current(std::move(internal_path)),
#if defined(PREDEF_PLATFORM_UNIX)
	  _pDir(nullptr), _rc(1)
#else
	_fh(INVALID_HANDLE_VALUE), _rc(1)
#endif
	{
#if defined(PREDEF_PLATFORM_UNIX)
		path p(_current);
		p.make_file ();


		_pDir = opendir(p.to_string ().c_str());

		if (!_pDir) file::handle_last_error (_current);

		_next();
#else
	path p(_current);
	p.makeDirectory();
	std::string findPath = p.toString();
	findPath.append("*");
	std::wstring uFindPath;
	FileImpl::convertPath(findPath, uFindPath);

	_fh = FindFirstFileW(uFindPath.c_str(), &_fd);
	if (_fh == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_NO_MORE_FILES)
			File::handleLastError(path);
	}
	else
	{
		UnicodeConverter::toUTF8(_fd.cFileName, _current);
		if (_current == "." || _current == "..")
			next();
	}
#endif
	}
	const std::string& physical_dir_iterator_ops::_next () {
#if defined(PREDEF_PLATFORM_UNIX)
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
#else
		do
	{
		_current.clear();
		if (FindNextFileW(_fh, &_fd) != 0)
		{
			UnicodeConverter::toUTF8(_fd.cFileName, _current);
		}
	}
	while (_current == "." || _current == "..");
	return _current;
#endif
	}

	physical_dir_iterator_ops::~physical_dir_iterator_ops() {
#if defined(PREDEF_PLATFORM_UNIX)
		if (_pDir) closedir(_pDir);
#else
		if (_fh != INVALID_HANDLE_VALUE)
		FindClose(_fh);
#endif
	}

	const std::string &physical_dir_iterator_ops::get () const {
		return _current;
	}

	const std::string &physical_dir_iterator_ops::next () {
		return _next();
	}
}