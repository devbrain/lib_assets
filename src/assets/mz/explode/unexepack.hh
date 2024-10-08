#ifndef EXPLODE_UNEXEPACK_HH_
#define EXPLODE_UNEXEPACK_HH_

#include "exe_file.hh"

namespace neutrino::assets::mz {
	class unexepack {
	 public:
		explicit unexepack (input_exe_file& inp);

		static bool accept (input_exe_file& inp) {
			return inp.is_exepack ();
		}

		void unpack (output_exe_file& oexe);
		[[nodiscard]] uint32_t decomp_size () const;
	 private:
		enum header_t {
			eREAL_START_OFFSET,
			eREAL_START_SEGMENT,
			eMEM_START_SEGMENT,
			eUNPACKER_LEN,
			eREAL_STACK_OFFSET,
			eREAL_STACK_SEGMENT,
			eDEST_LEN,
			eSKIP_LEN,
			eSIGNATURE,

			eMAX_HEADER_VAL
		};
	 private:
		io::input& m_file;
		[[maybe_unused]] input_exe_file& m_exe_file;

		uint16_t m_header[eMAX_HEADER_VAL];

		uint32_t m_exe_data_start;
		[[maybe_unused]] uint32_t m_extra_data_start;
		[[maybe_unused]] uint32_t m_packed_data_len;
		[[maybe_unused]] uint32_t m_rellocs_start;
	};
} // ns explode

#endif
