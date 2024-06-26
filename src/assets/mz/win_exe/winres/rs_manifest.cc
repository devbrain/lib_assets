#include <sstream>
#include <assets/resources/exe/winres/rs_manifest.hh>
#include <assets/resources/exe/winres/resource_directory.hh>
#include "mz/win_exe/windows_pe_file.hh"

namespace neutrino::assets {
	void windows_rs_manifest::_text(const char* t) {
		m_text = t;
	}

	// ------------------------------------------------------------------
	const std::string& windows_rs_manifest::text() const {
		return m_text;
	}

	// ------------------------------------------------------------------
	void windows_resource_traits<windows_rs_manifest>::load(const ms_file& file, const windows_resource& rn, windows_rs_manifest& out) {
		const std::size_t file_size = file.file_size();
		auto offs = file.offset_in_file(rn.offset());

		if (offs >= file_size) {
			return;
		}

		bsw::istream_wrapper stream(file.stream(), offs, rn.size());
		std::vector <char> data(rn.size());
		stream.read(data.data(), data.size());
		out.m_text.assign(data.data(), rn.size());
	}
} // ns pefile
