//
// Created by igor on 1/12/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_HH_

#include <assets/assets_export.h>
#include <assets/datastore/file_unix.hh>
#include <vector>

namespace neutrino::assets::datastore {
	class path;

	class ASSETS_EXPORT file : private FileImpl
		/// The File class provides methods for working with a file.
		///
		/// Regarding paths passed to the various methods, note that
		/// platform-specific limitations regarding maximum length
		/// of the entire path and its components apply.
		///
		/// On Windows, the implementation tries to work around the rather low
		/// 260 characters MAX_PATH limit by adding the "\\?\" prefix if
		/// a path is absolute and exceeds MAX_PATH characters in length.
		/// Note that various limitations regarding usage of the "\\?\"
		/// prefix apply in that case, e.g. the path must
		/// not contain relative components ("." and "..") and must not
		/// use the forward slash ("/") as directory separator.
	{
	 public:
		typedef FileSizeImpl file_size_t;

		enum link_type_t
			/// Type of link for link_to().
		{
			LINK_HARD = 0, /// hard link
			LINK_SYMBOLIC = 1  /// symbolic link
		};

		enum options_t
			/// Options for File Copy/Movement
		{
			OPT_FAIL_ON_OVERWRITE = OPT_FAIL_ON_OVERWRITE_IMPL
		};

		file ();
		/// Creates the file.

		explicit file (const std::string& pth);
		/// Creates the file.

		explicit file (const char* pth);
		/// Creates the file.

		explicit file (const path& pth);
		/// Creates the file.

		file (const file& f);
		/// Copy constructor.

		~file () override;
		/// Destroys the file.

		file& operator= (const file& f);
		/// Assignment operator.

		file& operator= (const std::string& pth);
		/// Assignment operator.

		file& operator= (const char* pth);
		/// Assignment operator.

		file& operator= (const path& pth);
		/// Assignment operator.

		void swap (file& f);
		/// Swaps the file with another one.

		[[nodiscard]] const std::string& get_path () const;
		/// Returns the path.

		[[nodiscard]] bool exists () const;
		/// Returns true iff the file exists.

		[[nodiscard]] bool can_read () const;
		/// Returns true iff the file is readable.

		[[nodiscard]] bool can_write () const;
		/// Returns true iff the file is writeable.

		[[nodiscard]] bool can_execute () const;
		/// Returns true iff the file is executable.
		///
		/// On Windows, the file must have
		/// the extension ".EXE" to be executable.
		/// On Unix platforms, the executable permission
		/// bit must be set.

		[[nodiscard]] bool is_file () const;
		/// Returns true iff the file is a regular file.

		[[nodiscard]] bool is_link () const;
		/// Returns true iff the file is a symbolic link.

		[[nodiscard]] bool is_directory () const;
		/// Returns true iff the file is a directory.

		[[nodiscard]] bool is_device () const;
		/// Returns true iff the file is a device.

		[[nodiscard]] bool is_hidden () const;
		/// Returns true if the file is hidden.
		///
		/// On Windows platforms, the file's hidden
		/// attribute is set for this to be true.
		///
		/// On Unix platforms, the file name must
		/// begin with a period for this to be true.



		[[nodiscard]] file_size_t get_size () const;
		/// Returns the size of the file in bytes.

		file& set_size (file_size_t size);
		/// Sets the size of the file in bytes. Can be used
		/// to truncate a file.

		file& set_writeable (bool flag = true);
		/// Makes the file writeable (if flag is true), or
		/// non-writeable (if flag is false) by setting the
		/// file's flags in the filesystem accordingly.

		file& set_read_only (bool flag = true);
		/// Makes the file non-writeable (if flag is true), or
		/// writeable (if flag is false) by setting the
		/// file's flags in the filesystem accordingly.

		file& set_executable (bool flag = true);
		/// Makes the file executable (if flag is true), or
		/// non-executable (if flag is false) by setting
		/// the file's permission bits accordingly.
		///
		/// Does nothing on Windows.

		void copy_to (const std::string& pth, int options = 0) const;
		/// Copies the file (or directory) to the given path.
		/// The target path can be a directory.
		///
		/// A directory is copied recursively.
		/// If options is set to OPT_FAIL_ON_OVERWRITE the Method throws an FileExists Exception
		/// if the File already exists.

		void move_to (const std::string& pth, int options = 0);
		/// Copies the file (or directory) to the given path and
		/// removes the original file. The target path can be a directory.
		/// If options is set to OPT_FAIL_ON_OVERWRITE the Method throws an FileExists Exception
		/// if the File already exists.

		void rename_to (const std::string& pth, int options = 0);
		/// Renames the file to the new name.
		/// If options is set to OPT_FAIL_ON_OVERWRITE the Method throws an FileExists Exception
		/// if the File already exists.

		void link_to (const std::string& pth, link_type_t type = LINK_SYMBOLIC) const;
		/// Creates a link (symbolic or hard, depending on type argument)
		/// at the given path to the file or directory.
		///
		/// May not be supported on all platforms.
		/// Furthermore, some operating systems do not allow creating
		/// hard links to directories.

		void remove (bool recursive = false);
		/// Deletes the file. If recursive is true and the
		/// file is a directory, recursively deletes all
		/// files in the directory.

		bool create_file ();
		/// Creates a new, empty file in an atomic operation.
		/// Returns true if the file has been created and false
		/// if the file already exists. Throws an exception if
		/// an error occurs.

		bool create_directory ();
		/// Creates a directory. Returns true if the directory
		/// has been created and false if it already exists.
		/// Throws an exception if an error occurs.

		void create_directories ();
		/// Creates a directory (and all parent directories
		/// if necessary).

		void list (std::vector<std::string>& files) const;
		/// Fills the vector with the names of all
		/// files in the directory.

		void list (std::vector<file>& files) const;
		/// Fills the vector with the names of all
		/// files in the directory.

		[[nodiscard]] file_size_t total_space () const;
		/// Returns the total size in bytes of the partition containing this path.

		[[nodiscard]] file_size_t usable_space () const;
		/// Returns the number of usable free bytes on the partition containing this path.

		[[nodiscard]] file_size_t free_space () const;
		/// Returns the number of free bytes on the partition containing this path.

		bool operator== (const file& f) const;
		bool operator!= (const file& f) const;
		bool operator< (const file& f) const;
		bool operator<= (const file& f) const;
		bool operator> (const file& f) const;
		bool operator>= (const file& f) const;

		static void handle_last_error (const std::string& pth);
		/// For internal use only. Throws an appropriate
		/// exception for the last file-related error.

	 protected:
		void copy_directory (const std::string& pth, int options = 0) const;
		/// Copies a directory. Used internally by copy_to().
	};

//
// inlines
//
	inline const std::string& file::get_path () const {
		return get_path_impl ();
	}

	inline bool file::operator== (const file& f) const {
		return get_path_impl () == f.get_path_impl ();
	}

	inline bool file::operator!= (const file& f) const {
		return get_path_impl () != f.get_path_impl ();
	}

	inline bool file::operator< (const file& f) const {
		return get_path_impl () < f.get_path_impl ();
	}

	inline bool file::operator<= (const file& f) const {
		return get_path_impl () <= f.get_path_impl ();
	}

	inline bool file::operator> (const file& f) const {
		return get_path_impl () > f.get_path_impl ();
	}

	inline bool file::operator>= (const file& f) const {
		return get_path_impl () >= f.get_path_impl ();
	}

	inline void swap (file& f1, file& f2) {
		f1.swap (f2);
	}

}

#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_FILE_HH_
