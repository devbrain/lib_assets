#ifndef __PEFILE_RS_MANIFEST_HPP__
#define __PEFILE_RS_MANIFEST_HPP__

#include <string>
#include "resource_directory.hh"
#include "pefile.hh"

namespace assets::pefile {
	class ASSETS_EXPORT manifest_c {
	 public:
		static constexpr int resource_id () {
			return 24;
		}

		static constexpr bool singleton () {
			return true;
		}

	 public:
		[[nodiscard]] const std::string& text () const;

		static void load (const file_c& file, const resource_c& rn, manifest_c& out);
		void parse (std::string& name, std::string& description,
					std::string& level, std::string& ui) const;
	 private:
		void _text (const char* t);
	 public:
		std::string m_text;

	};
	// ===================================================

} // ns pefile

#endif
