#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RESOURCE_DIRECTORY_HPP_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RESOURCE_DIRECTORY_HPP_

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <tuple>
#include <type_traits>

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
		friend  ASSETS_EXPORT bool operator<(const windows_resource_name& a, const windows_resource_name& b);

		public:
			windows_resource_name() = default;
			explicit windows_resource_name(int id);
			explicit windows_resource_name(const std::wstring& name);

			[[nodiscard]] bool is_id() const;
			[[nodiscard]] int id() const;
			void id(int x);

			[[nodiscard]] std::wstring name() const;
			void name(const std::wstring& x);

		private:
			std::pair <int, std::wstring> m_value;
	};

	// =============================================================================
	ASSETS_EXPORT bool operator<(const windows_resource_name& a, const windows_resource_name& b);
	ASSETS_EXPORT std::ostream& operator<<(std::ostream& os, const windows_resource_name& a);

	// =============================================================================
	class ASSETS_EXPORT windows_resource {
		public:
			windows_resource();

			[[nodiscard]] int language_code() const;
			void language_code(int x);

			[[nodiscard]] uint32_t offset() const;
			void offset(uint32_t x);

			[[nodiscard]] uint32_t size() const;
			void size(uint32_t x);

			[[nodiscard]] const windows_resource_name& name() const;
			void name(windows_resource_name& rn);

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

	template<typename T>
	struct windows_resource_traits;

	class ASSETS_EXPORT windows_resource_directory {
		friend class detail::resource_dir_builder;

		public:
			using second_level_t = std::multimap <windows_resource_name, windows_resource>;
			using first_level_t = std::map <windows_resource_name, second_level_t>;
			using iterator = second_level_t::const_iterator;

		public:
			windows_resource_directory();
			windows_resource_directory(windows_resource_directory&& other) noexcept ;
			~windows_resource_directory();

			class ASSETS_EXPORT names_iterator {
				friend class windows_resource_directory;

				public:
					names_iterator& operator++();
					bool operator==(const names_iterator& a) const;
					bool operator!=(const names_iterator& a) const;
					const windows_resource_name* operator->() const;
					const windows_resource_name& operator*() const;

				private:
					using itr_t = first_level_t::const_iterator;
					explicit names_iterator(itr_t i);

					itr_t m_itr;
			};

		public:
			[[nodiscard]] iterator begin(const windows_resource_name& rn) const;
			[[nodiscard]] iterator end(const windows_resource_name& rn) const;
			[[nodiscard]] bool exists(int id) const;

			[[nodiscard]] names_iterator names_begin() const;
			[[nodiscard]] names_iterator names_end() const;


		template <typename ResType>
		[[nodiscard]] bool exists() const {
			for (auto i = names_begin(); i != names_end(); ++i) {
				if (i->is_id() && i->id() == windows_resource_traits<ResType>::id) {
					return begin(*i) != end(*i);
				}
			}
			return false;
		}

		template <typename ResType>
		std::enable_if_t<windows_resource_traits<ResType>::singleton, std::optional<std::tuple<windows_resource_name, ResType>>>
		load() const {
			for (auto i = names_begin(); i != names_end(); ++i) {
				if (i->is_id() && i->id() == windows_resource_traits<ResType>::id) {
					for (auto j = begin(*i); j != end(*i); ++j) {
						ResType out;
						windows_resource_traits<ResType>::load(*m_file, j->second, out);
						return std::make_tuple(j->first, out);
					}
				}
			}
			return std::nullopt;
		}

		template <typename ResType>
		std::enable_if_t<!windows_resource_traits<ResType>::singleton, std::vector<std::tuple<windows_resource_name, ResType>>>
		load() const {
			std::vector<std::tuple<windows_resource_name, ResType>> result;
			for (auto i = names_begin(); i != names_end(); ++i) {
				if (i->is_id() && i->id() == windows_resource_traits<ResType>::id) {
					for (auto j = begin(*i); j != end(*i); ++j) {
						ResType out;
						windows_resource_traits<ResType>::load(*m_file, j->second, out);
						result.emplace_back(std::make_tuple(j->first, out));
					}
				}
			}
			return result;
		}

		protected:
			first_level_t m_dir;
			std::unique_ptr <ms_file> m_file;
	};



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
