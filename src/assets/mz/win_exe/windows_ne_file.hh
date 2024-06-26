//
// Created by igor on 6/26/24.
//

#ifndef  NEFILE_HH
#define  NEFILE_HH

#include <vector>
#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string>
#include <assets/resources/exe/winres/resource_directory.hh>
#include <assets/assets_export.h>
#include "mz/win_exe/ms_file.hh"

namespace neutrino::assets {
	struct tname_info {
		uint16_t rnOffset;
		uint16_t rnLength;
		uint16_t rnFlags;
		uint16_t rnID;
		uint16_t rnHandle;
		uint16_t rnUsage;
		std::optional <std::string> name;
	};

	struct type_info {
		uint16_t rtTypeID;
		uint16_t rtResourceCount;
		uint32_t rtReserved;
		std::optional <std::string> name;
		std::vector <tname_info> rtNameInfo;
	};

	class ASSETS_EXPORT windows_ne_file : public ms_file {
		public:
			explicit windows_ne_file(std::istream& is);
			[[nodiscard]] uint16_t get_align_shift() const;
			[[nodiscard]] const std::vector <type_info>& get_types_info() const;
			void build_resources(windows_resource_directory& out) const;
			[[nodiscard]] std::istream& stream() const override;
			[[nodiscard]] std::size_t file_size() const override;
			[[nodiscard]] std::size_t offset_in_file(uint32_t res_offset) const override;
		private:
			std::istream& m_stream;
			std::size_t m_file_size;
			uint16_t m_align_shift;
			std::vector <type_info> m_types_info;
	};
}

#endif
