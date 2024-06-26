#include "pe_resource_builder.hh"
#include "windows_pe_file.hh"
#include "windows_ne_file.hh"

namespace neutrino::assets {
	namespace detail {
		resource_dir_builder::resource_dir_builder(windows_resource_directory& rd)
			: m_dir(rd.m_dir) {
		}

		// ------------------------------------------------------------------------
		void resource_dir_builder::start_main_entry(const windows_resource_name& rcname) {
			m_curr.clear();
			m_curr_main_entry = rcname;
		}

		// -----------------------------------------------------------------------
		void resource_dir_builder::end_main_entry() {
			m_dir.insert(first_level_t::value_type(m_curr_main_entry, m_curr));
		}

		// -----------------------------------------------------------------------
		void resource_dir_builder::start_sub_entry(const windows_resource_name& rcname) {
			m_curr_sub_entry = rcname;
		}

		// -----------------------------------------------------------------------
		void resource_dir_builder::add_to_sub_entry(windows_resource& res) {
			res.name(m_curr_sub_entry);
			m_curr.insert(second_level_t::value_type(m_curr_sub_entry, res));
		}

		// -----------------------------------------------------------------------
		void resource_dir_builder::end_sub_entry() {
		}
		// =======================================================================
		struct IMAGE_RESOURCE_DIRECTORY_ENTRY {
			[[nodiscard]] bool valid() const {
				return offset_info.OffsetToData != 0;
			}

			union {
				struct {
					uint32_t NameOffset: 31;
					uint32_t NameIsString: 1;
				} info;

				uint32_t Name;
				uint16_t Id;
			} name_info;

			union {
				uint32_t OffsetToData;

				struct {
					uint32_t OffsetToDirectory: 31;
					uint32_t DataIsDirectory: 1;
				} info;
			} offset_info;
		};

		static_assert(sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)
		              == 8, "sizeof of IMAGE_RESOURCE_DIRECTORY_ENTRY struct should be 8 bytes");

		// ======================================================================================
		struct IMAGE_RESOURCE_DIRECTORY {
			uint32_t Characteristics;
			// Theoretically this field could hold flags for the resource, but appears to always be 0.
			uint32_t TimeDateStamp; // 	The time / date stamp describing the creation time of the resource.
			// Theoretically these fields would hold a version number for the resource.These field appear to always be set to 0.
			uint16_t MajorVersion;
			uint16_t MinorVersion;
			uint16_t NumberOfNamedEntries;
			// The number of array elements that use names and that follow this structure.
			uint16_t NumberOfIdEntries;
			// The number of array elements that use integer IDs, and which follow this structure.
		};

		static_assert(
			sizeof(IMAGE_RESOURCE_DIRECTORY) == 16, "sizeof of IMAGE_RESOURCE_DIRECTORY struct should be 16 bytes");

		// ========================================================================
		uint32_t count_entries(bsw::istream_wrapper& is) {
			IMAGE_RESOURCE_DIRECTORY rd{};
			is >> rd;
			return rd.NumberOfIdEntries + rd.NumberOfNamedEntries;
		}

		// ------------------------------------------------------------------------
		std::wstring read_string(bsw::istream_wrapper& stream, uint32_t offs) {
			auto curr = stream.current_pos();
			stream.seek(offs);

			uint16_t size;
			stream >> size;

			std::wstring s;
			if (size < stream.size_to_end()) {
				std::vector<char> data(size*2);
				stream.read(data.data(), data.size());
				union {
					const char* bytes;
					const wchar_t* words;
				} u;
				u.bytes = data.data();
				s.assign(u.words, u.words + size);
			}

			stream.seek(curr);
			return s;
		}

		// ------------------------------------------------------------------------
		void
		parse_resource_directory(uint32_t rs_rva, uint32_t offs, bsw::istream_wrapper& stream, int depth,
		                         resource_dir_builder& rdb) {
			if (depth > 1) {
				std::cerr << "max depth reached" << std::endl;
				return;
			}

			stream.seek(offs);
			uint32_t count = count_entries(stream);
			for (uint32_t i = 0; i < count; i++) {
				IMAGE_RESOURCE_DIRECTORY_ENTRY entry{};
				stream >> entry;
				windows_resource_name rname;
				if (depth == 0) {
					if (entry.name_info.info.NameIsString) {
						rname.name(read_string(stream, entry.name_info.info.NameOffset));
					} else {
						rname.id(entry.name_info.Id);
					}
				}
				if (entry.offset_info.info.DataIsDirectory) {
					if (depth == 0) {
						rdb.start_sub_entry(rname);
						auto pos = stream.current_pos();
						parse_resource_directory(rs_rva, entry.offset_info.info.OffsetToDirectory, stream,
						                         depth + 1, rdb);
						stream.seek(pos);
						rdb.end_sub_entry();
					}
				} else {
					if (depth == 1) {
						auto current = stream.current_pos();

						stream.seek(entry.offset_info.info.OffsetToDirectory);

						uint32_t data_rva;
						uint32_t size;

						stream >> data_rva;
						stream >> size;
						stream.seek(current);

						windows_resource rc;
						rc.language_code(entry.name_info.Id);
						//	rc.offset(data_rva - rs_rva);
						rc.offset(data_rva);
						rc.size(size);
						rdb.add_to_sub_entry(rc);
					}
				}
			}
		}

		// ------------------------------------------------------------------------
		void
		create_res_dir(uint32_t rs_rva, bsw::istream_wrapper& stream, const windows_pe_file& pefile,
		               resource_dir_builder& rdb) {
			const uint32_t count = count_entries(stream);
			for (uint32_t i = 0; i < count; i++) {
				IMAGE_RESOURCE_DIRECTORY_ENTRY entry{};
				stream >> entry;

				windows_resource_name rname;
				if (entry.name_info.info.NameIsString) {
					rname.name(read_string(stream, entry.name_info.info.NameOffset));
				} else {
					rname.id(entry.name_info.Id);
				}
				if (entry.offset_info.info.DataIsDirectory) {
					rdb.start_main_entry(rname);
					auto pos = stream.current_pos();
					parse_resource_directory(rs_rva, entry.offset_info.info.OffsetToDirectory, stream, 0, rdb);
					stream.seek(pos);
					rdb.end_main_entry();
				}
			}
		}
	} // ns detail
	// ============================================================================
	void build_resources(const windows_pe_file& pefile, windows_resource_directory& rd) {
		const std::size_t file_size = pefile.file_size();
		const auto& entry = pefile.optional_header().DataDirectory[(int)DataDirectory::Resource];
		const uint32_t rc_offs = pefile.translate_rva(entry.VirtualAddress);
		if (rc_offs == 0 || (rc_offs + entry.Size) > file_size) {
			return;
		}
		bsw::istream_wrapper stream(pefile.stream(), rc_offs, entry.Size);
		detail::resource_dir_builder builder(rd);
		create_res_dir(entry.VirtualAddress, stream, pefile, builder);
	}

	void build_resources (const windows_ne_file& pefile, windows_resource_directory& rd) {
		pefile.build_resources(rd);
	}
} // ns pefile
