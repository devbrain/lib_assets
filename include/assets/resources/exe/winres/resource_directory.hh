#ifndef __PEFILE_RESOURCE_DIRECTORY_HPP__
#define __PEFILE_RESOURCE_DIRECTORY_HPP__

#include <string>
#include <map>
#include <cstdint>

#include <assets/assets_export.h>

namespace assets::pefile {
	static constexpr int RCT_NEWRESOURCE = 0x2000;
	enum resource_type_t {
		CURSOR = 1,
		BITMAP = 2,
		ICON = 3,
		MENU = 4,
		DIALOG = 5,
		STRING = 6,
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

	class windows_pe_file;

	class ASSETS_EXPORT resource_name {
		friend bool operator< (const resource_name& a, const resource_name& b);
	 public:
		resource_name () = default;
		explicit resource_name (int id);
		explicit resource_name (const std::wstring& name);

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
	ASSETS_EXPORT bool operator< (const resource_name& a, const resource_name& b);
	ASSETS_EXPORT std::ostream& operator<< (std::ostream& os, const resource_name& a);

	// =============================================================================
	class ASSETS_EXPORT  resource {
	 public:
		resource ();

		[[nodiscard]] int language_code () const;
		void language_code (int x);

		[[nodiscard]] uint32_t offset () const;
		void offset (uint32_t x);

		[[nodiscard]] uint32_t size () const;
		void size (uint32_t x);

		[[nodiscard]] const resource_name& name () const;
		void name (resource_name& rn);

		[[nodiscard]] std::size_t offset_in_file (const windows_pe_file& f) const;

	 private:
		int m_language_code;
		uint32_t m_offset;
		uint32_t m_size;
		resource_name m_rn;
	};
	// =============================================================================
	namespace detail {
		class resource_dir_builder;
	}

	class ASSETS_EXPORT resource_dir {
		friend class detail::resource_dir_builder;

	 public:
		using second_level_t = std::multimap<resource_name, resource>;
		using first_level_t = std::map<resource_name, second_level_t>;
		using iterator = second_level_t::const_iterator;
	 public:
		class names_iterator {
			friend class resource_dir;

		 public:
			names_iterator& operator++ ();
			bool operator== (const names_iterator& a);
			bool operator!= (const names_iterator& a);
			const resource_name* operator-> () const;
			const resource_name& operator* () const;
		 private:
			using itr_t = first_level_t::const_iterator;
		 private:
			explicit names_iterator (itr_t i);
		 private:
			itr_t m_itr;
		};

	 public:
		[[nodiscard]] iterator begin (const resource_name& rn) const;
		[[nodiscard]] iterator end (const resource_name& rn) const;
		[[nodiscard]] bool exists (int id) const;

		[[nodiscard]] names_iterator names_begin () const;
		[[nodiscard]] names_iterator names_end () const;
	 protected:
		first_level_t m_dir;
	};

} // ns pefile

#endif
