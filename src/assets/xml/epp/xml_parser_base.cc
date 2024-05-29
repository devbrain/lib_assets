#include <iostream>
#include "xml/epp/xml_parser_base.hh"

namespace bsw
{
	namespace xml
	{
		namespace detail
		{
			parser_base::parser_base()
				: m_is_owner (true)
			{
				m_parser = XML_ParserCreate(nullptr);
			}
			// ------------------------------------------------------------------
			parser_base::parser_base(XML_Parser other)
				: m_parser(other),
				m_is_owner(true)
			{

			}
			// ------------------------------------------------------------------
			parser_base::parser_base(const std::string& encoding)
				: m_is_owner(true)
			{
				m_parser = XML_ParserCreate(encoding.empty () ? nullptr : encoding.c_str ());
			}
			// ------------------------------------------------------------------
			parser_base::parser_base(const std::string& encoding, char ns_separator)
				: m_is_owner (true)
			{
				m_parser = XML_ParserCreateNS(encoding.empty() ? nullptr : encoding.c_str(), ns_separator);
			}
			// ------------------------------------------------------------------
			parser_base::parser_base(parser_base&& other)
				: m_parser(other.m_parser),
				m_is_owner(other.m_is_owner)
			{
				other.m_is_owner = false;
			}
			// ------------------------------------------------------------------
			parser_base& parser_base::operator = (parser_base&& other)
			{
				m_parser = other.m_parser;
				m_is_owner = other.m_is_owner;
				other.m_is_owner = false;
				return *this;
			}
			// ------------------------------------------------------------------
			parser_base::~parser_base()
			{
				if (m_is_owner)
				{
					XML_ParserFree(m_parser);
				}
			}
			// --------------------------------------------------------------------
			void parser_base::default_fallback()
			{
				XML_DefaultCurrent(m_parser);
			}
			// --------------------------------------------------------------------
			void parser_base::encoding(const std::string& enc)
			{
				XML_SetEncoding(m_parser, enc.c_str());
			}
			// --------------------------------------------------------------------
			void parser_base::reset(const std::string& encoding)
			{
				XML_ParserReset(m_parser, encoding.empty() ? nullptr : encoding.c_str());
			}
			// --------------------------------------------------------------------
			void parser_base::reset()
			{
				XML_ParserReset(m_parser, nullptr);
			}
			// ---------------------------------------------------------------------
			XML_Error parser_base::use_foreign_dtd(bool v)
			{
				return XML_UseForeignDTD(m_parser, v ? XML_TRUE : XML_FALSE);
			}
			// ---------------------------------------------------------------------
			void parser_base::base(const std::string& v)
			{
				XML_SetBase(m_parser, v.c_str());
			}
			// ---------------------------------------------------------------------
			std::string parser_base::base() const
			{
				if (auto b = XML_GetBase(m_parser))
				{
					return b;
				}
				return "";
			}
			// ----------------------------------------------------
			XML_Status parser_base::set_encoding(const std::string& encoding)
			{
				return XML_SetEncoding(m_parser, encoding.c_str());
			}
			// ----------------------------------------------------
			XML_Status parser_base::parse(const char* input, std::size_t len)
			{
				return feed(input, len, true);
			}
			// ----------------------------------------------------
			XML_Status parser_base::parse(const std::string& input)
			{
				return parse(input.c_str(), input.size());
			}
			// ----------------------------------------------------
			XML_Status parser_base::feed(const char* input, std::size_t len, bool is_final)
			{
				return XML_Parse(m_parser, input, static_cast <int>(len), is_final);
			}
			// ----------------------------------------------------
			XML_Status parser_base::stop(bool is_resumable)
			{
				return XML_StopParser(m_parser, is_resumable ? XML_TRUE : XML_FALSE);
			}
			// ----------------------------------------------------
			XML_Status parser_base::resume()
			{
				return XML_ResumeParser(m_parser);
			}
			// ----------------------------------------------------
			void parser_base::status(XML_Parsing& parser_status, bool& is_final) const
			{
				XML_ParsingStatus s;
				XML_GetParsingStatus(m_parser, &s);
				parser_status = s.parsing;
				is_final = s.finalBuffer == XML_TRUE ? true : false;
			}
			// ---------------------------------------------------------------
			XML_Parser parser_base::_create_external_entity(const std::string& context,
				const std::string& encoding)
			{
				return XML_ExternalEntityParserCreate(m_parser, context.c_str(), encoding.c_str());
			}
			// ---------------------------------------------------------------
			int parser_base::param_entity_policy(XML_ParamEntityParsing policy)
			{
				return XML_SetParamEntityParsing(m_parser, policy);
			}
			// ---------------------------------------------------------------
			XML_Error parser_base::error() const
			{
				return XML_GetErrorCode(m_parser);
			}
			// ----------------------------------------------------------------
			unsigned parser_base::line() const
			{
				return XML_GetCurrentLineNumber(m_parser);
			}
			// ----------------------------------------------------------------
			unsigned parser_base::column() const
			{
				return XML_GetCurrentColumnNumber(m_parser);
			}
			// ----------------------------------------------------------------
			unsigned parser_base::byte_count() const
			{
				return XML_GetCurrentByteIndex(m_parser);
			}
			// ----------------------------------------------------------------
			std::string parser_base::describe_error(XML_Error e)
			{
				const char* err = XML_ErrorString(e);
				if (err)
				{
					return err;
				}
				return "";
			}
			// -----------------------------------------------------------------
			void parser_base::version(int& major, int& minor, int& micro)
			{
				XML_Expat_Version v = XML_ExpatVersionInfo();
				major = v.major;
				minor = v.minor;
				micro = v.micro;
			}
			// -----------------------------------------------------------------
			void parser_base::_user_data(void* obj)
			{
				XML_SetUserData(m_parser, obj);
			}
		} // ns detail
	} // ns xml
} // ns bsw

std::ostream& operator << (std::ostream& os, XML_Error e)
{
	switch (e)
	{
	case XML_ERROR_NONE: os << "XML_ERROR_NONE"; break;
	case XML_ERROR_NO_MEMORY: os << "XML_ERROR_NO_MEMORY"; break;
	case XML_ERROR_SYNTAX: os << "XML_ERROR_SYNTAX"; break;
	case XML_ERROR_NO_ELEMENTS: os << "XML_ERROR_NO_ELEMENTS"; break;
	case XML_ERROR_INVALID_TOKEN: os << "XML_ERROR_INVALID_TOKEN"; break;
	case XML_ERROR_UNCLOSED_TOKEN: os << "XML_ERROR_UNCLOSED_TOKEN"; break;
	case XML_ERROR_PARTIAL_CHAR: os << "XML_ERROR_PARTIAL_CHAR"; break;
	case XML_ERROR_TAG_MISMATCH: os << "XML_ERROR_TAG_MISMATCH"; break;
	case XML_ERROR_DUPLICATE_ATTRIBUTE: os << "XML_ERROR_DUPLICATE_ATTRIBUTE"; break;
	case XML_ERROR_JUNK_AFTER_DOC_ELEMENT: os << "XML_ERROR_JUNK_AFTER_DOC_ELEMENT"; break;
	case XML_ERROR_PARAM_ENTITY_REF: os << "XML_ERROR_PARAM_ENTITY_REF"; break;
	case XML_ERROR_UNDEFINED_ENTITY: os << "XML_ERROR_UNDEFINED_ENTITY"; break;
	case XML_ERROR_RECURSIVE_ENTITY_REF: os << "XML_ERROR_RECURSIVE_ENTITY_REF"; break;
	case XML_ERROR_ASYNC_ENTITY: os << "XML_ERROR_ASYNC_ENTITY"; break;
	case XML_ERROR_BAD_CHAR_REF: os << "XML_ERROR_BAD_CHAR_REF"; break;
	case XML_ERROR_BINARY_ENTITY_REF: os << "XML_ERROR_BINARY_ENTITY_REF"; break;
	case XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF: os << "XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF"; break;
	case XML_ERROR_MISPLACED_XML_PI: os << "XML_ERROR_MISPLACED_XML_PI"; break;
	case XML_ERROR_UNKNOWN_ENCODING: os << "XML_ERROR_UNKNOWN_ENCODING"; break;
	case XML_ERROR_INCORRECT_ENCODING: os << "XML_ERROR_INCORRECT_ENCODING"; break;
	case XML_ERROR_UNCLOSED_CDATA_SECTION: os << "XML_ERROR_UNCLOSED_CDATA_SECTION"; break;
	case XML_ERROR_EXTERNAL_ENTITY_HANDLING: os << "XML_ERROR_EXTERNAL_ENTITY_HANDLING"; break;
	case XML_ERROR_NOT_STANDALONE: os << "XML_ERROR_NOT_STANDALONE"; break;
	case XML_ERROR_UNEXPECTED_STATE: os << "XML_ERROR_UNEXPECTED_STATE"; break;
	case XML_ERROR_ENTITY_DECLARED_IN_PE: os << "XML_ERROR_ENTITY_DECLARED_IN_PE"; break;
	case XML_ERROR_FEATURE_REQUIRES_XML_DTD: os << "XML_ERROR_FEATURE_REQUIRES_XML_DTD"; break;
	case XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING: os << "XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING"; break;
	case XML_ERROR_UNBOUND_PREFIX: os << "XML_ERROR_UNBOUND_PREFIX"; break;
	case XML_ERROR_UNDECLARING_PREFIX: os << "XML_ERROR_UNDECLARING_PREFIX"; break;
	case XML_ERROR_INCOMPLETE_PE: os << "XML_ERROR_INCOMPLETE_PE"; break;
	case XML_ERROR_XML_DECL: os << "XML_ERROR_XML_DECL"; break;
	case XML_ERROR_TEXT_DECL: os << "XML_ERROR_TEXT_DECL"; break;
	case XML_ERROR_PUBLICID: os << "XML_ERROR_PUBLICID"; break;
	case XML_ERROR_SUSPENDED: os << "XML_ERROR_SUSPENDED"; break;
	case XML_ERROR_NOT_SUSPENDED: os << "XML_ERROR_NOT_SUSPENDED"; break;
	case XML_ERROR_ABORTED: os << "XML_ERROR_ABORTED"; break;
	case XML_ERROR_FINISHED: os << "XML_ERROR_FINISHED"; break;
	case XML_ERROR_SUSPEND_PE: os << "XML_ERROR_SUSPEND_PE"; break;
	case XML_ERROR_RESERVED_PREFIX_XML: os << "XML_ERROR_RESERVED_PREFIX_XML"; break;
	case XML_ERROR_RESERVED_PREFIX_XMLNS: os << "XML_ERROR_RESERVED_PREFIX_XMLNS"; break;
	case XML_ERROR_RESERVED_NAMESPACE_URI: os << "XML_ERROR_RESERVED_NAMESPACE_URI"; break;
	}
	return os;
}