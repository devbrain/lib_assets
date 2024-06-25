#ifndef PEFILE_EXEFILE_HH_
#define PEFILE_EXEFILE_HH_

#include <memory>

#include "pefile.hh"
#include "imports_parser.hh"
#include "assets/resources/exe/resource_directory.hh"
#include "assets/resources/exe/rs_version.hh"
#include "assets/resources/exe/rs_manifest.hh"
#include "assets/resources/exe/rs_icon_group.hh"
#include "assets/resources/exe/rs_string_table.hh"
#include "assets/resources/exe/rs_dialog.hh"
#include "assets/resources/exe/rs_message_table.hh"
#include "clr.hh"

#include <assets/assets_export.h>

namespace assets::pefile {
	class ASSETS_EXPORT exe_file_c {
	 public:
		explicit exe_file_c (std::istream& is);

		[[nodiscard]] bool compressed () const;
		[[nodiscard]] const char* entry_point (std::size_t sz) const;
		[[nodiscard]] bool is_64 () const;
		[[nodiscard]] bool is_gui () const;

		[[nodiscard]] const imports_table_t& imports () const;
		void load_imports ();
		[[nodiscard]] std::size_t file_size () const;
		[[nodiscard]] const char* file_data () const;
		[[nodiscard]] std::size_t import_size () const;

		template <typename T>
		bool load_resource (T& out) const;

		template <typename T>
		bool load_resource (std::vector<T>& out) const;

		template <typename T>
		void load_resource (const resource_c& rn, T& out) const {
			T::load (*m_pefile, rn, out);
		}

		[[nodiscard]] std::vector<resource_name_c> resources () const;

		[[nodiscard]] std::vector<SECTION> sections () const;
		[[nodiscard]] float entropy (const SECTION& s) const;
		[[nodiscard]] const char* read_section (const SECTION& s) const;
		[[nodiscard]] const OPTIONAL_HEADER& optional_header () const;

		clr_c load_clr ();

		[[nodiscard]] std::size_t get_overlay_offset () const;
		[[nodiscard]] const resource_dir_c& resource_directory () const;
		[[nodiscard]] std::size_t resource_offset (const resource_c& rn) const;
	 private:
		void _load ();
	 private:
		std::unique_ptr<windows_pe_file> m_pefile;
		bool m_is_gui{};
		bool m_is_64_bit{};
		resource_dir_c m_resource_directory;
		imports_table_t m_imports;
		uint32_t m_entry_point{};

	};

	template <typename T>
	bool exe_file_c::load_resource (T& out) const {
		auto ni = m_resource_directory.names_begin ();
		auto end = m_resource_directory.names_end ();
		bool found = false;
		for (auto i = ni; i != end; ++i) {
			if (i->is_id () && i->id () == T::resource_id ()) {
				for (auto j = m_resource_directory.begin (*i); j != m_resource_directory.end (*i); j++) {
					found = true;
					T::load (*m_pefile, j->second, out);
				}
			}
		}
		return found;
	}

	// ---------------------------------------------------------------------
	template <typename T>
	bool exe_file_c::load_resource (std::vector<T>& out) const {
		auto ni = m_resource_directory.names_begin ();
		auto end = m_resource_directory.names_end ();
		for (auto i = ni; i != end; ++i) {
			if (i->is_id () && i->id () == T::resource_id ()) {
				for (auto j = m_resource_directory.begin (*i); j != m_resource_directory.end (*i); j++) {
					T v;
					T::load (*m_pefile, j->second, v);
					out.push_back (v);
				}
			}
		}
		return !out.empty ();
	}
} // ns pefile

#endif
