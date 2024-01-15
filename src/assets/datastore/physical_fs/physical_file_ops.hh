//
// Created by igor on 1/15/24.
//

#ifndef ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_FILE_OPS_HH_
#define ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_FILE_OPS_HH_

#include <assets/datastore/file_operations.hh>

namespace neutrino::assets::datastore::physical_fs {
	class physical_file_ops : public file_operations {
	 public:
		explicit physical_file_ops (std::string pth);


		void setPathImpl (const std::string& path) override;

		[[nodiscard]] bool existsImpl () const override;
		[[nodiscard]] bool canReadImpl () const override;
		[[nodiscard]] bool canWriteImpl () const override;
		[[nodiscard]] bool canExecuteImpl () const override;
		[[nodiscard]] bool isFileImpl () const override;
		[[nodiscard]] bool isDirectoryImpl () const override;
		[[nodiscard]] bool isLinkImpl () const override;
		[[nodiscard]] bool isDeviceImpl () const override;
		[[nodiscard]] bool isHiddenImpl () const override;

		[[nodiscard]] FileSizeImpl getSizeImpl () const override;
		void setSizeImpl (FileSizeImpl size) override;
		void setWriteableImpl (bool flag = true) override;
		void setExecutableImpl (bool flag = true) override;
		void copyToImpl (const std::string& path, int options) const override;
		void renameToImpl (const std::string& path, int options) override;
		void linkToImpl (const std::string& path, int type) const override;
		void removeImpl () override;
		[[nodiscard]] bool createFileImpl () override;
		[[nodiscard]] bool createDirectoryImpl () override;
		[[nodiscard]] FileSizeImpl totalSpaceImpl () const override;
		[[nodiscard]] FileSizeImpl usableSpaceImpl () const override;
		[[nodiscard]] FileSizeImpl freeSpaceImpl () const override;
		void handleLastErrorImpl (const std::string& path) const override;
	};
}

#endif //ASSETS_SRC_ASSETS_DATASTORE_PHYSICAL_FS_PHYSICAL_FILE_OPS_HH_
