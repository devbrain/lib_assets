//
// Created by igor on 1/12/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_UNIX_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_UNIX_HH_

#include <cstdint>
#include <string>
#include <chrono>

#include <assets/assets_export.h>

namespace neutrino::assets::datastore {
	class FileImpl {
	 protected:
		enum Options {
			OPT_FAIL_ON_OVERWRITE_IMPL = 0x01
		};

		typedef uint64_t FileSizeImpl;

		FileImpl ();
		explicit FileImpl (std::string path);
		virtual ~FileImpl ();
		void swapImpl (FileImpl &file);
		void setPathImpl (const std::string &path);
		[[nodiscard]] const std::string &get_path_impl () const;
		[[nodiscard]] bool existsImpl () const;
		[[nodiscard]] bool canReadImpl () const;
		[[nodiscard]] bool canWriteImpl () const;
		[[nodiscard]] bool canExecuteImpl () const;
		[[nodiscard]] bool isFileImpl () const;
		[[nodiscard]] bool isDirectoryImpl () const;
		[[nodiscard]] bool isLinkImpl () const;
		[[nodiscard]] bool isDeviceImpl () const;
		[[nodiscard]] bool isHiddenImpl () const;

		[[nodiscard]] FileSizeImpl getSizeImpl () const;
		void setSizeImpl (FileSizeImpl size);
		void setWriteableImpl (bool flag = true);
		void setExecutableImpl (bool flag = true);
		void copyToImpl (const std::string &path, int options = 0) const;
		void renameToImpl (const std::string &path, int options = 0);
		void linkToImpl (const std::string &path, int type) const;
		void removeImpl ();
		bool createFileImpl ();
		bool createDirectoryImpl ();
		[[nodiscard]] FileSizeImpl totalSpaceImpl () const;
		[[nodiscard]] FileSizeImpl usableSpaceImpl () const;
		[[nodiscard]] FileSizeImpl freeSpaceImpl () const;
		static void handleLastErrorImpl (const std::string &path);

	 private:
		std::string _path;
	};

//
// inlines
//
	inline const std::string &FileImpl::get_path_impl () const {
		return _path;
	}

}

#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_UNIX_HH_
