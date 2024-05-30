#ifndef EXPLODE_KNOWLEDGE_DYNAMICS_HH_
#define EXPLODE_KNOWLEDGE_DYNAMICS_HH_

#include "exe_file.hh"

namespace assets::mz
{
  class  knowledge_dynamics
  {
  public:
    explicit knowledge_dynamics(input_exe_file& inp);

    static bool accept(input_exe_file& inp);

    void unpack(output_exe_file& oexe);
    [[nodiscard]] uint32_t decomp_size() const;
  private:
    io::input&          m_file;

    uint32_t m_expected_size;
    uint32_t m_code_offs;
		
    uint16_t m_header[exe_file::MAX_HEADER_VAL];
  };
} // ns explode

#endif
