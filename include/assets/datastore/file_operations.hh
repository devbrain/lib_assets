//
// Created by igor on 1/15/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_OPERATIONS_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_OPERATIONS_HH_

#include <cstdint>
#include <string>

#include <assets/assets_export.h>

namespace neutrino::assets::datastore {

	class ASSETS_EXPORT file_operations {
	 public:
		enum Options {
			OPT_FAIL_ON_OVERWRITE_IMPL = 0x01
		};

		typedef uint64_t FileSizeImpl;


		explicit file_operations (std::string pth);
		virtual ~file_operations ();


		virtual void setPathImpl (const std::string& path) = 0;
		[[nodiscard]] virtual const std::string& get_path_impl () const;
		[[nodiscard]] virtual bool existsImpl () const  = 0;
		[[nodiscard]] virtual bool canReadImpl () const  = 0;
		[[nodiscard]] virtual bool canWriteImpl () const  = 0;
		[[nodiscard]] virtual bool canExecuteImpl () const  = 0;
		[[nodiscard]] virtual bool isFileImpl () const  = 0;
		[[nodiscard]] virtual bool isDirectoryImpl () const  = 0;
		[[nodiscard]] virtual bool isLinkImpl () const  = 0;
		[[nodiscard]] virtual bool isDeviceImpl () const  = 0;
		[[nodiscard]] virtual bool isHiddenImpl () const  = 0;

		[[nodiscard]] virtual FileSizeImpl getSizeImpl () const = 0;
		virtual void setSizeImpl (FileSizeImpl size) = 0;
		virtual void setWriteableImpl (bool flag = true) = 0;
		virtual void setExecutableImpl (bool flag = true) = 0;
		virtual void copyToImpl (const std::string& path, int options = 0) const = 0;
		virtual void renameToImpl (const std::string& path, int options = 0) = 0;
		virtual void linkToImpl (const std::string& path, int type) const = 0;
		virtual void removeImpl () = 0;
		[[nodiscard]] virtual bool createFileImpl () = 0;
		[[nodiscard]] virtual bool createDirectoryImpl () = 0;
		[[nodiscard]] virtual FileSizeImpl totalSpaceImpl () const = 0;
		[[nodiscard]] virtual FileSizeImpl usableSpaceImpl () const = 0;
		[[nodiscard]] virtual FileSizeImpl freeSpaceImpl () const = 0;
		virtual void handleLastErrorImpl (const std::string& path) const = 0;
	 protected:
		std::string m_path;
	};


}
#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_OPERATIONS_HH_
