//
// Created by igor on 06/08/2021.
//

#ifndef NEUTRINO_XML_READER_HH
#define NEUTRINO_XML_READER_HH

#include <optional>
#include <bsw/spimpl.h>
#include "world/tmx/reader.hh"

namespace pugi {
	class xml_node;
}

namespace neutrino::assets::tmx {
	class xml_reader final : public reader {
		public:
			static xml_reader load(const char* xml, std::size_t size, const char* root_elt_name);
			explicit xml_reader(const pugi::xml_node& node);
			xml_reader(xml_reader&&) = default;
			~xml_reader() override;

			[[nodiscard]] std::string get_text() const;

		private:
			void parse_each_element(visitor_t func) const override;
			void parse_many_elements(const char* name, visitor_t func) const override;
			void parse_one_element(const char* name, visitor_t func) const override;
			bool has_child(const char* name) const override;
			std::optional <std::string> get_attribute_value(const char* name) const override;

		private:
			struct impl;
			spimpl::unique_impl_ptr <impl> m_pimpl;
	};
}

#endif //NEUTRINO_XML_READER_HH
