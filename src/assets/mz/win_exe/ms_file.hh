//
// Created by igor on 6/26/24.
//

#ifndef  MS_FILE_HH
#define  MS_FILE_HH
#include <iosfwd>
#include <cstddef>

namespace neutrino::assets {
	class ms_file {
		public:
			virtual ~ms_file() = default;
			[[nodiscard]] virtual std::istream& stream() const = 0;
			[[nodiscard]] virtual std::size_t file_size() const = 0;
			[[nodiscard]] virtual std::size_t offset_in_file(uint32_t res_offset) const = 0;
			[[nodiscard]] virtual bool is_pe() const = 0;
	};
}

#endif
