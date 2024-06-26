#ifndef __PEFILE_RESOURCE_DIRECTORY_HPP__
#define __PEFILE_RESOURCE_DIRECTORY_HPP__

#include <string>
#include <map>
#include <cstdint>

#include <assets/assets_export.h>

namespace neutrino::assets {
	static constexpr int RCT_NEWRESOURCE = 0x2000;
	enum windows_resource_type {
		CURSOR = 1,
		BITMAP = 2,
		ICON = 3,
		MENU = 4,
		DIALOG = 5,
		STRINGTABLE = 6,
		FONTDIR = 7,
		FONT = 8,
		ACCELERATORS = 9,
		RCDATA = 10,
		MESSAGETABLE = 11,
		GROUP_CURSOR = 12,
		GROUP_ICON = 14,
		VERSION = 16,
		NEWBITMAP = (BITMAP | RCT_NEWRESOURCE),
		NEWMENU = (MENU | RCT_NEWRESOURCE),
		NEWDIALOG = (DIALOG | RCT_NEWRESOURCE),
		DLGINCLUDE = 17,
		PLUGPLAY = 19,
		VXD = 20,
		ANICURSOR = 21,
		ANIICON = 22,
		HTML = 23,
		MANIFEST = 24
	};

	ASSETS_EXPORT std::string to_string(windows_resource_type resource_type);
	ASSETS_EXPORT std::wstring to_wstring(windows_resource_type resource_type);

	class ms_file;

	class ASSETS_EXPORT windows_resource_name {
		friend bool operator< (const windows_resource_name& a, const windows_resource_name& b);
	 public:
		windows_resource_name () = default;
		explicit windows_resource_name (int id);
		explicit windows_resource_name (const std::wstring& name);

		[[nodiscard]] bool is_id () const;
		[[nodiscard]] int id () const;
		void id (int x);

		[[nodiscard]] std::wstring name () const;
		void name (const std::wstring& x);

		[[nodiscard]] bool is_special () const;

	 private:
		std::pair<int, std::wstring> m_value;
	};

	// =============================================================================
	ASSETS_EXPORT bool operator< (const windows_resource_name& a, const windows_resource_name& b);
	ASSETS_EXPORT std::ostream& operator<< (std::ostream& os, const windows_resource_name& a);

	// =============================================================================
	class ASSETS_EXPORT  windows_resource {
	 public:
		windows_resource ();

		[[nodiscard]] int language_code () const;
		void language_code (int x);

		[[nodiscard]] uint32_t offset () const;
		void offset (uint32_t x);

		[[nodiscard]] uint32_t size () const;
		void size (uint32_t x);

		[[nodiscard]] const windows_resource_name& name () const;
		void name (windows_resource_name& rn);

	 private:
		int m_language_code;
		uint32_t m_offset;
		uint32_t m_size;
		windows_resource_name m_rn;
	};
	// =============================================================================
	namespace detail {
		class resource_dir_builder;
	}

	class ASSETS_EXPORT windows_resource_directory {
		friend class detail::resource_dir_builder;

	 public:
		using second_level_t = std::multimap<windows_resource_name, windows_resource>;
		using first_level_t = std::map<windows_resource_name, second_level_t>;
		using iterator = second_level_t::const_iterator;
	 public:
		class names_iterator {
			friend class windows_resource_directory;

		 public:
			names_iterator& operator++ ();
			bool operator== (const names_iterator& a);
			bool operator!= (const names_iterator& a);
			const windows_resource_name* operator-> () const;
			const windows_resource_name& operator* () const;
		 private:
			using itr_t = first_level_t::const_iterator;
		 private:
			explicit names_iterator (itr_t i);
		 private:
			itr_t m_itr;
		};

	 public:
		[[nodiscard]] iterator begin (const windows_resource_name& rn) const;
		[[nodiscard]] iterator end (const windows_resource_name& rn) const;
		[[nodiscard]] bool exists (int id) const;

		[[nodiscard]] names_iterator names_begin () const;
		[[nodiscard]] names_iterator names_end () const;
	 protected:
		first_level_t m_dir;
	};


	template <typename T>
	struct windows_resource_traits;

#define d_ASSETS_WINDOWS_RESOURCE_TRAITS(T, ID, SINGLETON)							\
	class ASSETS_EXPORT T;															\
	template <>																		\
	struct ASSETS_EXPORT windows_resource_traits<T> {								\
		static constexpr int id = ID;												\
		static constexpr auto singleton = SINGLETON;								\
		static void load(const ms_file& file, const windows_resource& rn, T& out);	\
	}

#define d_ASSETS_ADD_WINDOWS_RESOURCE_LOADER(T)	friend struct windows_resource_traits<T>
} // ns pefile

#endif
