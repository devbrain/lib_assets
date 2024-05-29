/**
 * @file xml.hpp
 * Wrapper around XML expat library
 */
#ifndef ARES_BSW_XML_HPP
#define ARES_BSW_XML_HPP

#include <sstream>
#include <stdexcept>

#include "xml/epp/xml_handler_iface.hh"
#include "xml/epp/xml_parser_base.hh"

namespace bsw
{
	namespace xml
	{
		template <typename Parser>
		class parser : public detail::parser_base
		{
		public:
			explicit parser(Parser& obj)
			{
				_register_callbacks();
				_user_data(&obj);
			}

			parser(Parser& obj, const std::string& encoding)
				: detail::parser_base (encoding)
			{
				_register_callbacks();
				_user_data(&obj);
			}
		private:
			void _register_callbacks()
			{
				detail::register_XmlDeclHandler <Parser>(m_parser);
				detail::register_StartElementHandler <Parser>(m_parser);
				detail::register_EndElementHandler <Parser>(m_parser);
				detail::register_CharacterDataHandler <Parser>(m_parser);
				detail::register_ProcessingInstructionHandler <Parser>(m_parser);
				detail::register_StartCdataSectionHandler <Parser>(m_parser);
				detail::register_EndCdataSectionHandler <Parser>(m_parser);
				detail::register_AttlistDeclHandler <Parser>(m_parser);
				detail::register_DefaultHandler <Parser>(m_parser);
				detail::register_StartDoctypeDeclHandler <Parser>(m_parser);
				detail::register_EndDoctypeDeclHandler <Parser>(m_parser);
				detail::register_EntityDeclHandler <Parser>(m_parser);
				detail::register_NotationDeclHandler <Parser>(m_parser);
				detail::register_StartNamespaceDeclHandler <Parser>(m_parser);
				detail::register_EndNamespaceDeclHandler <Parser>(m_parser);
				detail::register_SkippedEntityHandler <Parser>(m_parser);
				detail::register_NotStandaloneHandler <Parser>(m_parser);
			}
		};
		// ============================================================
		template <typename Parser>
		void parse(Parser& obj, const char* input, std::size_t len)
		{
			parser <Parser> expat_parser(obj);
			auto status = expat_parser.parse(input, len);
			if (status == XML_STATUS_ERROR)
			{
				XML_Error err = expat_parser.error();
				std::ostringstream os;
				os << "XML parsing error [" << err << "] "
					<< detail::parser_base::describe_error(err)
					<< "\n"
					<< "Line: " << expat_parser.line() << " Col: " << expat_parser.column();
				throw std::runtime_error(os.str());
			}
		}
		// ----------------------------------------------------------------
		template <typename Parser>
		void parse(Parser& obj, const std::string& text)
		{
			parse(obj, text.c_str(), text.size());
		}
	}
} // ns bsw

#endif
