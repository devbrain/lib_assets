#ifndef EXPLODE_UNLZEXE_HH_
#define EXPLODE_UNLZEXE_HH_

#include "explode/mz/exe_file.hh"

namespace assets::mz
{
    class  unlzexe
    {
    public:
        explicit unlzexe(input_exe_file& inp);

        void unpack(output_exe_file& oexe);

        static bool accept(input_exe_file& inp);

        uint32_t decomp_size() const;
    private:
        enum header_t
        {
            eIP,                    // 0
            eCS,                    // 1
            eSP,                    // 2
            eSS,                    // 3
            eCOMPRESSED_SIZE,       // 4
            eINC_SIZE,              // 5
            eDECOMPRESSOR_SIZE,     // 6
            eCHECKSUM,              // 7

            eHEADER_MAX
        };
    private:
        io::input& m_file;
        input_exe_file& m_exe_file;

        int m_ver;
        uint16_t m_header[eHEADER_MAX];

        uint32_t m_rellocs_offset;
        uint32_t m_code_offset;
    };
} // ns explode

#endif
