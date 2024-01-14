//
// Created by igor on 1/12/24.
//

#include <chrono>
#include <thread>
#include <assets/datastore/file.hh>
#include <assets/datastore/directory_iterator.hh>
#include <bsw/exception.hh>

namespace neutrino::assets::datastore {
	file::file () = default;

	file::file (const std::string& pth)
		: FileImpl (pth) {
	}

	file::file (const char* pth)
		: FileImpl (std::string (pth)) {
	}

	file::file (const path& pth)
		: FileImpl (pth.to_string ()) {
	}

	file::file (const file& f)
		: FileImpl (f.get_path_impl ()) {
	}

	file::~file () = default;

	file& file::operator= (const file& f) {
		setPathImpl (f.get_path_impl ());
		return *this;
	}

	file& file::operator= (const std::string& path) {
		setPathImpl (path);
		return *this;
	}

	file& file::operator= (const char* path) {
		ENFORCE(path);
		setPathImpl (path);
		return *this;
	}

	file& file::operator= (const path& pth) {
		setPathImpl (pth.to_string ());
		return *this;
	}

	void file::swap (file& f) {
		swapImpl (f);
	}

	bool file::exists () const {
		return existsImpl ();
	}

	bool file::can_read () const {
		return canReadImpl ();
	}

	bool file::can_write () const {
		return canWriteImpl ();
	}

	bool file::can_execute () const {
		return canExecuteImpl ();
	}

	bool file::is_file () const {
		return isFileImpl ();
	}

	bool file::is_directory () const {
		return isDirectoryImpl ();
	}

	bool file::is_link () const {
		return isLinkImpl ();
	}

	bool file::is_device () const {
		return isDeviceImpl ();
	}

	bool file::is_hidden () const {
		return isHiddenImpl ();
	}

	file::file_size_t file::get_size () const {
		return getSizeImpl ();
	}

	file& file::set_size (file_size_t size) {
		setSizeImpl (size);
		return *this;
	}

	file& file::set_writeable (bool flag) {
		setWriteableImpl (flag);
		return *this;
	}

	file& file::set_read_only (bool flag) {
		setWriteableImpl (!flag);
		return *this;
	}

	file& file::set_executable (bool flag) {
		setExecutableImpl (flag);
		return *this;
	}

	void file::copy_to (const std::string& pth, int options) const {
		path src (get_path_impl ());
		path dest (pth);
		file destFile (pth);
		if ((destFile.exists () && destFile.is_directory ()) || dest.is_directory ()) {
			dest.make_directory ();
			dest.set_file_name (src.get_file_name ());
		}
		if (is_directory ()) {
			copy_directory (dest.to_string (), options);
		} else {
			copyToImpl (dest.to_string (), options);
		}
	}

	void file::copy_directory (const std::string& pth, int options) const {
		file target (pth);
		target.create_directories ();

		path src (get_path_impl ());
		src.make_file ();
		directory_iterator it (src);
		directory_iterator end;
		for (; it != end; ++it) {
			it->copy_to (pth, options);
		}
	}

	void file::move_to (const std::string& pth, int options) {
		copy_to (pth, options);
		remove (true);
		setPathImpl (pth);
	}

	void file::rename_to (const std::string& pth, int options) {
		renameToImpl (pth, options);
		setPathImpl (pth);
	}

	void file::link_to (const std::string& pth, link_type_t type) const {
		linkToImpl (pth, type);
	}

	void file::remove (bool recursive) {
		if (recursive && !is_link () && is_directory ()) {
			std::vector<file> files;
			list (files);
			for (auto& f : files) {
				f.remove (true);
			}

			// Note: On Windows, removing a directory may not succeed at first
			// try because deleting files is not a synchronous operation. Files
			// are merely marked as deleted, and actually removed at a later time.
			//
			// An alternate strategy would be moving files to a different directory
			// first (on the same drive, but outside the deleted tree), and marking
			// them as hidden, before deleting them, but this could lead to other issues.
			// So we simply retry after some time until we succeed, or give up.
			removeImpl ();
			/*
			int retry = 8;
			long sleep = 10;
			while (retry > 0) {
				try {
					removeImpl ();
					retry = 0;
				}
				catch (std::exception &) {
					if (--retry == 0) { throw; }
					std::this_thread::sleep_for (std::chrono::milliseconds (sleep));
					sleep *= 2;
				}
			}
			 */
		} else {
			removeImpl ();
		}
	}

	bool file::create_file () {
		return createFileImpl ();
	}

	bool file::create_directory () {
		return createDirectoryImpl ();
	}

	void file::create_directories () {
		if (!exists ()) {
			path p (get_path_impl ());
			p.make_directory ();
			if (p.depth () > 1) {
				p.make_parent ();
				file f (p);
				f.create_directories ();
			}
			try {
				createDirectoryImpl ();
			}
			catch (bsw::exception&) {
			}
		}
	}

	void file::list (std::vector<std::string>& files) const {
		files.clear ();
		directory_iterator it (*this);
		directory_iterator end;
		while (it != end) {
			files.push_back (it.name ());
			++it;
		}
	}

	file::file_size_t file::total_space () const {
		return totalSpaceImpl ();
	}

	file::file_size_t file::usable_space () const {
		return usableSpaceImpl ();
	}

	file::file_size_t file::free_space () const {
		return freeSpaceImpl ();
	}

	void file::list (std::vector<file>& files) const {
		files.clear ();
		directory_iterator it (*this);
		directory_iterator end;
		while (it != end) {
			files.push_back (*it);
			++it;
		}
	}

	void file::handle_last_error (const std::string& pth) {
		handleLastErrorImpl (pth);
	}

}