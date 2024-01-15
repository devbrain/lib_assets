//
// Created by igor on 1/15/24.
//

#ifndef ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_UNIX_FILE_OPS_HH_
#define ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_UNIX_FILE_OPS_HH_

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

#if defined(PREDEF_OS_SOLARIS) || defined(PREDEF_OS_QNX)
#define STATFSFN statvfs
#define STATFSSTRUCT statvfs
#else
#define STATFSFN statfs
#define STATFSSTRUCT statfs
#endif

namespace neutrino::assets::datastore::physical_fs {
	physical_file_ops::physical_file_ops (std::string pth)
		: file_operations (std::move (pth)) {
		std::string::size_type n = m_path.size ();
		if (n > 1 && m_path[n - 1] == '/') {
			m_path.resize (n - 1);
		}
	}

	void physical_file_ops::setPathImpl (const std::string& pth) {
		m_path = pth;
		std::string::size_type n = m_path.size ();
		if (n > 1 && m_path[n - 1] == '/') {
			m_path.resize (n - 1);
		}
	}

	bool physical_file_ops::existsImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		return stat (m_path.c_str (), &st) == 0;
	}

	bool physical_file_ops::canReadImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) == 0) {
			if (st.st_uid == geteuid ()) {
				return (st.st_mode & S_IRUSR) != 0;
			} else if (st.st_gid == getegid ()) {
				return (st.st_mode & S_IRGRP) != 0;
			} else {
				return (st.st_mode & S_IROTH) != 0 || geteuid () == 0;
			}
		} else { handleLastErrorImpl (m_path); }
		return false;
	}

	bool physical_file_ops::canWriteImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) == 0) {
			if (st.st_uid == geteuid ()) {
				return (st.st_mode & S_IWUSR) != 0;
			} else if (st.st_gid == getegid ()) {
				return (st.st_mode & S_IWGRP) != 0;
			} else {
				return (st.st_mode & S_IWOTH) != 0 || geteuid () == 0;
			}
		} else { handleLastErrorImpl (m_path); }
		return false;
	}

	bool physical_file_ops::canExecuteImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) == 0) {
			if (st.st_uid == geteuid () || geteuid () == 0) {
				return (st.st_mode & S_IXUSR) != 0;
			} else if (st.st_gid == getegid ()) {
				return (st.st_mode & S_IXGRP) != 0;
			} else {
				return (st.st_mode & S_IXOTH) != 0;
			}
		} else { handleLastErrorImpl (m_path); }
		return false;
	}

	bool physical_file_ops::isFileImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) == 0) {
			return S_ISREG(st.st_mode);
		} else {
			handleLastErrorImpl (m_path);
		}
		return false;
	}

	bool physical_file_ops::isDirectoryImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) == 0) {
			return S_ISDIR(st.st_mode);
		} else {
			handleLastErrorImpl (m_path);
		}
		return false;
	}

	bool physical_file_ops::isLinkImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (lstat (m_path.c_str (), &st) == 0) {
			return S_ISLNK(st.st_mode);
		} else {
			handleLastErrorImpl (m_path);
		}
		return false;
	}

	bool physical_file_ops::isDeviceImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) == 0) {
			return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
		} else {
			handleLastErrorImpl (m_path);
		}
		return false;
	}

	bool physical_file_ops::isHiddenImpl () const {
		ENFORCE (!m_path.empty ())
		path p (m_path);
		p.make_file ();

		return p.get_file_name ()[0] == '.';
	}

	physical_file_ops::FileSizeImpl physical_file_ops::getSizeImpl () const {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) == 0) {
			return st.st_size;
		} else {
			handleLastErrorImpl (m_path);
		}
		return 0;
	}

	void physical_file_ops::setSizeImpl (FileSizeImpl size) {
		ENFORCE (!m_path.empty ())

		if (truncate (m_path.c_str (), size) != 0) {
			handleLastErrorImpl (m_path);
		}
	}

	void physical_file_ops::setWriteableImpl (bool flag) {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) != 0) {
			handleLastErrorImpl (m_path);
		}
		mode_t mode;
		if (flag) {
			mode = st.st_mode | S_IWUSR;
		} else {
			mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
			mode = st.st_mode & ~wmask;
		}
		if (chmod (m_path.c_str (), mode) != 0) {
			handleLastErrorImpl (m_path);
		}
	}

	void physical_file_ops::setExecutableImpl (bool flag) {
		ENFORCE (!m_path.empty ())

		struct stat st{};
		if (stat (m_path.c_str (), &st) != 0) {
			handleLastErrorImpl (m_path);
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
		if (chmod (m_path.c_str (), mode) != 0) {
			handleLastErrorImpl (m_path);
		}
	}

	void physical_file_ops::copyToImpl (const std::string& path, int options) const {
		ENFORCE (!m_path.empty ())

		int sd = open (m_path.c_str (), O_RDONLY);
		if (sd == -1) { handleLastErrorImpl (m_path); }

		struct stat st{};
		if (fstat (sd, &st) != 0) {
			close (sd);
			handleLastErrorImpl (m_path);
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
				handleLastErrorImpl (m_path);
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

	void physical_file_ops::renameToImpl (const std::string& path, int options) {
		ENFORCE (!m_path.empty ())

		struct stat st{};

		if (stat (path.c_str (), &st) == 0 && (options & OPT_FAIL_ON_OVERWRITE_IMPL))
			RAISE_EX("Path exists ", path);

		if (rename (m_path.c_str (), path.c_str ()) != 0) {
			handleLastErrorImpl (m_path);
		}
	}

	void physical_file_ops::linkToImpl (const std::string& path, int type) const {
		ENFORCE (!m_path.empty ())

		if (type == 0) {
			if (link (m_path.c_str (), path.c_str ()) != 0) {
				handleLastErrorImpl (m_path);
			}
		} else {
			if (symlink (m_path.c_str (), path.c_str ()) != 0) {
				handleLastErrorImpl (m_path);
			}
		}
	}

	void physical_file_ops::removeImpl () {
		ENFORCE (!m_path.empty ())

		int rc;
		if (!isLinkImpl () && isDirectoryImpl ()) {
			rc = rmdir (m_path.c_str ());
		} else {
			rc = unlink (m_path.c_str ());
		}
		if (rc) { handleLastErrorImpl (m_path); }
	}

	bool physical_file_ops::createFileImpl () {
		ENFORCE (!m_path.empty ())

		int n = open (m_path.c_str (),
					  O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		if (n != -1) {
			close (n);
			return true;
		}
		if (n == -1 && errno == EEXIST) {
			return false;
		} else {
			handleLastErrorImpl (m_path);
		}
		return false;
	}

	bool physical_file_ops::createDirectoryImpl () {
		ENFORCE (!m_path.empty ())

		if (existsImpl () && isDirectoryImpl ()) {
			return false;
		}
		if (mkdir (m_path.c_str (), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
			handleLastErrorImpl (m_path);
		}
		return true;
	}

	physical_file_ops::FileSizeImpl physical_file_ops::totalSpaceImpl () const {
		ENFORCE(!m_path.empty ())

		struct STATFSSTRUCT stats{};
		if (STATFSFN (const_cast<char*>(m_path.c_str ()), &stats) != 0) {
			handleLastErrorImpl (m_path);
		}

		return (FileSizeImpl)stats.f_blocks * (FileSizeImpl)stats.f_bsize;
	}

	physical_file_ops::FileSizeImpl physical_file_ops::usableSpaceImpl () const {
		ENFORCE(!m_path.empty ())

		struct STATFSSTRUCT stats{};
		if (STATFSFN (const_cast<char*>(m_path.c_str ()), &stats) != 0) {
			handleLastErrorImpl (m_path);
		}

		return (FileSizeImpl)stats.f_bavail * (FileSizeImpl)stats.f_bsize;
	}

	physical_file_ops::FileSizeImpl physical_file_ops::freeSpaceImpl () const {
		ENFORCE(!m_path.empty ())

		struct STATFSSTRUCT stats{};
		if (STATFSFN (const_cast<char*>(m_path.c_str ()), &stats) != 0) {
			handleLastErrorImpl (m_path);
		}

		return (FileSizeImpl)stats.f_bfree * (FileSizeImpl)stats.f_bsize;
	}

	void physical_file_ops::handleLastErrorImpl (const std::string& path) const {
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
#endif //ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_UNIX_FILE_OPS_HH_
