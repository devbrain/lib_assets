#include <sstream>
#include <assets/resources/exe/winres/rs_manifest.hh>
#include <assets/resources/exe/winres/resource_directory.hh>
#include "pefile.hh"

namespace assets::pefile {
	void manifest::_text(const char* t) {
		m_text = t;
	}

	// ------------------------------------------------------------------
	const std::string& manifest::text() const {
		return m_text;
	}

	// ------------------------------------------------------------------
	void manifest::load(const windows_pe_file& file, const resource& rn, manifest& out) {
		const std::size_t file_size = file.file_size();
		auto offs = rn.offset_in_file(file);

		if (offs >= file_size) {
			return;
		}

		bsw::istream_wrapper stream(file.stream(), offs, rn.size());
		std::vector <char> data(rn.size());
		stream.read(data.data(), data.size());
		out.m_text.assign(data.data(), rn.size());
	}
} // ns pefile
