#ifndef __PEFILE_RS_MANIFEST_HPP__
#define __PEFILE_RS_MANIFEST_HPP__

#include <string>
#include <assets/resources/exe/winres/resource_directory.hh>


namespace assets::pefile {
	class ASSETS_EXPORT manifest {
	 public:
		static constexpr int resource_id () {
			return 24;
		}

		static constexpr bool singleton () {
			return true;
		}

	 public:
		[[nodiscard]] const std::string& text () const;

		static void load (const windows_pe_file& file, const resource& rn, manifest& out);

	 private:
		void _text (const char* t);
	 public:
		std::string m_text;

	};
	// ===================================================

} // ns pefile

#endif
