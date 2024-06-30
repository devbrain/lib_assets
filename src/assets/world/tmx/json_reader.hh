//
// Created by igor on 06/08/2021.
//

#ifndef NEUTRINO_JSON_READER_HH
#define NEUTRINO_JSON_READER_HH

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

#include "world/tmx/reader.hh"
#include <bsw/spimpl.h>
#include <sdlpp/video/geometry.hh>
#include <nlohmann/json_fwd.hpp>


namespace neutrino::assets::tmx {
  class json_reader : public reader {
    public:
      static void assert_type (const char* type, const reader& elt);
      static json_reader load (const char* json, std::size_t size, const char* root_elt_name);
      explicit json_reader (const nlohmann::json& node);
      json_reader (json_reader&&) = default;
      ~json_reader () override;

      void iterate_data_array (const std::function<void (uint32_t v)>& f, const char* name = "data") const;
      void parse_each_element_of (const char* name, const visitor_t& func) const;
      bool has_element (const char* name) const noexcept;
      std::vector<sdl::point2f> parse_points (const char* name) const;
    private:
      void parse_each_element (visitor_t func) const override;
      void parse_many_elements (const char* name, visitor_t func) const override;
      void parse_one_element (const char* name, visitor_t func) const override;
      bool has_child (const char* name) const override;
      std::optional<std::string> get_attribute_value (const char* name) const override;
    private:
      struct impl;
      spimpl::unique_impl_ptr<impl> m_pimpl;
  };
}

#endif //NEUTRINO_JSON_READER_HH
