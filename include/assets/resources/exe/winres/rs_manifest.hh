#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_MANIFEST_HPP_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_MANIFEST_HPP_

#include <string>
#include <assets/resources/exe/winres/resource_directory.hh>


namespace neutrino::assets {
	d_ASSETS_WINDOWS_RESOURCE_TRAITS(windows_rs_manifest, MANIFEST, false);

	class ASSETS_EXPORT windows_rs_manifest {
	 d_ASSETS_ADD_WINDOWS_RESOURCE_LOADER(windows_rs_manifest);
	 public:
		[[nodiscard]] const std::string& text () const;
	 private:
		void _text (const char* t);
	 public:
		std::string m_text;

	};
	// ===================================================

} // ns pefile

#endif
