//
// Created by igor on 1/12/24.
//

#include <assets/datastore/file_unix.hh>
#include <assets/datastore/path.hh>
#include <bsw/predef.h>
#include <bsw/exception.hh>

#include <sys/stat.h>

#if defined(PREDEF_PLATFORM_BSD)
#include <sys/param.h>
#include <sys/mount.h>
#elif defined(PREDEF_OS_SOLARIS) || defined(PREDEF_OS_QNX)
#include <sys/statvfs.h>
#else
#include <sys/statfs.h>

#endif
#include <fcntl.h>
#include <cerrno>
#include <unistd.h>
#include <cstdio>
#include <utility>

#if defined(POCO_OS_SOLARIS) || defined(POCO_OS_QNX)
#define STATFSFN statvfs
#define STATFSSTRUCT statvfs
#else
#define STATFSFN statfs
#define STATFSSTRUCT statfs
#endif

namespace neutrino::assets::datastore {
	FileImpl::FileImpl () = default;

	FileImpl::FileImpl (std::string path)
		: _path (std::move(path)) {
		std::string::size_type n = _path.size ();
		if (n > 1 && _path[n - 1] == '/') {
			_path.resize (n - 1);
		}
	}

	FileImpl::~FileImpl () = default;

	void FileImpl::swapImpl (FileImpl &file) {
		std::swap (_path, file._path);
	}

	void FileImpl::setPathImpl (const std::string &path) {
		_path = path;
		std::string::size_type n = _path.size ();
		if (n > 1 && _path[n - 1] == '/') {
			_path.resize (n - 1);
		}
	}

	bool FileImpl::existsImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		return stat (_path.c_str (), &st) == 0;
	}

	bool FileImpl::canReadImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) == 0) {
			if (st.st_uid == geteuid ()) {
				return (st.st_mode & S_IRUSR) != 0;
			} else if (st.st_gid == getegid ()) {
				return (st.st_mode & S_IRGRP) != 0;
			} else {
				return (st.st_mode & S_IROTH) != 0 || geteuid () == 0;
			}
		} else { handleLastErrorImpl (_path); }
		return false;
	}

	bool FileImpl::canWriteImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) == 0) {
			if (st.st_uid == geteuid ()) {
				return (st.st_mode & S_IWUSR) != 0;
			} else if (st.st_gid == getegid ()) {
				return (st.st_mode & S_IWGRP) != 0;
			} else {
				return (st.st_mode & S_IWOTH) != 0 || geteuid () == 0;
			}
		} else { handleLastErrorImpl (_path); }
		return false;
	}

	bool FileImpl::canExecuteImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) == 0) {
			if (st.st_uid == geteuid () || geteuid () == 0) {
				return (st.st_mode & S_IXUSR) != 0;
			} else if (st.st_gid == getegid ()) {
				return (st.st_mode & S_IXGRP) != 0;
			} else {
				return (st.st_mode & S_IXOTH) != 0;
			}
		} else { handleLastErrorImpl (_path); }
		return false;
	}

	bool FileImpl::isFileImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) == 0) {
			return S_ISREG(st.st_mode);
		} else {
			handleLastErrorImpl (_path);
		}
		return false;
	}

	bool FileImpl::isDirectoryImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) == 0) {
			return S_ISDIR(st.st_mode);
		} else {
			handleLastErrorImpl (_path);
		}
		return false;
	}

	bool FileImpl::isLinkImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (lstat (_path.c_str (), &st) == 0) {
			return S_ISLNK(st.st_mode);
		} else {
			handleLastErrorImpl (_path);
		}
		return false;
	}

	bool FileImpl::isDeviceImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) == 0) {
			return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
		} else {
			handleLastErrorImpl (_path);
		}
		return false;
	}

	bool FileImpl::isHiddenImpl () const {
		ENFORCE (!_path.empty ())
		path p (_path);
		p.make_file ();

		return p.get_file_name ()[0] == '.';
	}

	FileImpl::FileSizeImpl FileImpl::getSizeImpl () const {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) == 0) {
			return st.st_size;
		} else {
			handleLastErrorImpl (_path);
		}
		return 0;
	}

	void FileImpl::setSizeImpl (FileSizeImpl size) {
		ENFORCE (!_path.empty ())

		if (truncate (_path.c_str (), size) != 0) {
			handleLastErrorImpl (_path);
		}
	}

	void FileImpl::setWriteableImpl (bool flag) {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) != 0) {
			handleLastErrorImpl (_path);
		}
		mode_t mode;
		if (flag) {
			mode = st.st_mode | S_IWUSR;
		} else {
			mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
			mode = st.st_mode & ~wmask;
		}
		if (chmod (_path.c_str (), mode) != 0) {
			handleLastErrorImpl (_path);
		}
	}

	void FileImpl::setExecutableImpl (bool flag) {
		ENFORCE (!_path.empty ())

		struct stat st{};
		if (stat (_path.c_str (), &st) != 0) {
			handleLastErrorImpl (_path);
		}
		mode_t mode;
		if (flag) {
			mode = st.st_mode | S_IXUSR;
			if (st.st_mode & S_IRGRP) {
				mode |= S_IXGRP;
			}
			if (st.st_mode & S_IROTH) {
				mode |= S_IXOTH;
			}
		} else {
			mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
			mode = st.st_mode & ~wmask;
		}
		if (chmod (_path.c_str (), mode) != 0) {
			handleLastErrorImpl (_path);
		}
	}

	void FileImpl::copyToImpl (const std::string &path, int options) const {
		ENFORCE (!_path.empty ())

		int sd = open (_path.c_str (), O_RDONLY);
		if (sd == -1) { handleLastErrorImpl (_path); }

		struct stat st{};
		if (fstat (sd, &st) != 0) {
			close (sd);
			handleLastErrorImpl (_path);
		}
		const long blockSize = st.st_blksize;
		int dd;
		if (options & OPT_FAIL_ON_OVERWRITE_IMPL) {
			dd = open (path.c_str (), O_CREAT | O_TRUNC | O_EXCL | O_WRONLY, st.st_mode);
		} else {
			dd = open (path.c_str (), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
		}
		if (dd == -1) {
			close (sd);
			handleLastErrorImpl (path);
		}
		std::vector<char> buffer (blockSize);
		try {
			ssize_t n;
			while ((n = read (sd, buffer.data (), blockSize)) > 0) {
				if (write (dd, buffer.data (), n) != n) {
					handleLastErrorImpl (path);
				}
			}
			if (n < 0) {
				handleLastErrorImpl (_path);
			}
		}
		catch (...) {
			close (sd);
			close (dd);
			throw;
		}
		close (sd);
		if (fsync (dd) != 0) {
			close (dd);
			handleLastErrorImpl (path);
		}
		if (close (dd) != 0) {
			handleLastErrorImpl (path);
		}
	}

	void FileImpl::renameToImpl (const std::string &path, int options) {
		ENFORCE (!_path.empty ())

		struct stat st{};

		if (stat (path.c_str (), &st) == 0 && (options & OPT_FAIL_ON_OVERWRITE_IMPL))
			RAISE_EX("Path exists ", path);

		if (rename (_path.c_str (), path.c_str ()) != 0) {
			handleLastErrorImpl (_path);
		}
	}

	void FileImpl::linkToImpl (const std::string &path, int type) const {
		ENFORCE (!_path.empty ())

		if (type == 0) {
			if (link (_path.c_str (), path.c_str ()) != 0) {
				handleLastErrorImpl (_path);
			}
		} else {
			if (symlink (_path.c_str (), path.c_str ()) != 0) {
				handleLastErrorImpl (_path);
			}
		}
	}

	void FileImpl::removeImpl () {
		ENFORCE (!_path.empty ())

		int rc;
		if (!isLinkImpl () && isDirectoryImpl ()) {
			rc = rmdir (_path.c_str ());
		} else {
			rc = unlink (_path.c_str ());
		}
		if (rc) { handleLastErrorImpl (_path); }
	}

	bool FileImpl::createFileImpl () {
		ENFORCE (!_path.empty ())

		int n = open (_path.c_str (),
					  O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		if (n != -1) {
			close (n);
			return true;
		}
		if (n == -1 && errno == EEXIST) {
			return false;
		} else {
			handleLastErrorImpl (_path);
		}
		return false;
	}

	bool FileImpl::createDirectoryImpl () {
		ENFORCE (!_path.empty ())

		if (existsImpl () && isDirectoryImpl ()) {
			return false;
		}
		if (mkdir (_path.c_str (), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
			handleLastErrorImpl (_path);
		}
		return true;
	}

	FileImpl::FileSizeImpl FileImpl::totalSpaceImpl () const {
		ENFORCE(!_path.empty ())

		struct STATFSSTRUCT stats{};
		if (STATFSFN (const_cast<char *>(_path.c_str ()), &stats) != 0) {
			handleLastErrorImpl (_path);
		}

		return (FileSizeImpl)stats.f_blocks * (FileSizeImpl)stats.f_bsize;
	}

	FileImpl::FileSizeImpl FileImpl::usableSpaceImpl () const {
		ENFORCE(!_path.empty ())

		struct STATFSSTRUCT stats{};
		if (STATFSFN (const_cast<char *>(_path.c_str ()), &stats) != 0) {
			handleLastErrorImpl (_path);
		}

		return (FileSizeImpl)stats.f_bavail * (FileSizeImpl)stats.f_bsize;
	}

	FileImpl::FileSizeImpl FileImpl::freeSpaceImpl () const {
		ENFORCE(!_path.empty ())

		struct STATFSSTRUCT stats{};
		if (STATFSFN (const_cast<char *>(_path.c_str ()), &stats) != 0) {
			handleLastErrorImpl (_path);
		}

		return (FileSizeImpl)stats.f_bfree * (FileSizeImpl)stats.f_bsize;
	}

	void FileImpl::handleLastErrorImpl (const std::string &path) {
		switch (errno) {
			case EIO: RAISE_EX("IO Error ", path, ",errno ", errno);
			case EPERM: RAISE_EX("insufficient permissions", path, ",errno ", errno);
			case EACCES: RAISE_EX("Access denied ", path, "errno", errno);
			case ENOENT: RAISE_EX("File not found ", path);
			case ENOTDIR: RAISE_EX("not a directory", path);
			case EISDIR: RAISE_EX("not a file", path);
			case EROFS: RAISE_EX("Read only path", path, "errno", errno);
			case EEXIST: RAISE_EX("File exists", path);
			case ENOSPC: RAISE_EX("no space left on device ", path);
			case EDQUOT: RAISE_EX("disk quota exceeded", path);
#if !defined(_AIX)
			case ENOTEMPTY: RAISE_EX("Directory is not empty", path);
#endif
			case ENAMETOOLONG: RAISE_EX ("Name too long", path);
			case ENFILE:
			case EMFILE: RAISE_EX("too many open files ", path, "errno", errno);
			default: RAISE_EX("IO Error ", path, ",errno ", errno);
		}
	}

}