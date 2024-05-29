#ifndef ARES_BSW_XML_PARSER_BASE_HH
#define ARES_BSW_XML_PARSER_BASE_HH

#include <string>
#include <iosfwd>
#include "xml/epp/expat_inc.hh"

namespace bsw
{
	namespace xml
	{
		namespace detail
		{
			class parser_base
			{
			public:
				parser_base();
				parser_base(const std::string& encoding);

				/* Constructs a new parser using the memory management suite referred to
				by memsuite. If memsuite is NULL, then use the standard library memory
				suite. If namespaceSeparator is non-NULL it creates a parser with
				namespace processing as described above. The character pointed at
				will serve as the namespace separator.

				All further memory operations used for the created parser will come from
				the given suite.
				*/
				parser_base(const std::string& encoding, char ns_separator);

				parser_base(const parser_base&) = delete;
				parser_base& operator = (const parser_base&) = delete;

				parser_base(parser_base&& other);
				parser_base& operator = (parser_base&& other);

				~parser_base();

				/* This can be called within a handler for a start element, end
				element, processing instruction or character data.  It causes the
				corresponding markup to be passed to the default handler.
				*/
				void default_fallback();

				/* This is equivalent to supplying an encoding argument to
				XML_ParserCreate. On success XML_SetEncoding returns non-zero,
				zero otherwise.
				Note: Calling XML_SetEncoding after XML_Parse or XML_ParseBuffer
				has no effect and returns XML_STATUS_ERROR.
				*/
				void encoding(const std::string& enc);

				/* Prepare a parser object to be re-used.  This is particularly
				valuable when memory allocation overhead is disproportionatly high,
				such as when a large number of small documnents need to be parsed.
				All handlers are cleared from the parser, except for the
				unknownEncodingHandler. The parser's external state is re-initialized
				except for the values of ns and ns_triplets.

				Added in Expat 1.95.3.
				*/
				void reset(const std::string& encoding);
				void reset();

				/* If useDTD == XML_TRUE is passed to this function, then the parser
				will assume that there is an external subset, even if none is
				specified in the document. In such a case the parser will call the
				externalEntityRefHandler with a value of NULL for the systemId
				argument (the publicId and context arguments will be NULL as well).
				Note: For the purpose of checking WFC: Entity Declared, passing
				useDTD == XML_TRUE will make the parser behave as if the document
				had a DTD with an external subset.
				Note: If this function is called, then this must be done before
				the first call to XML_Parse or XML_ParseBuffer, since it will
				have no effect after that.  Returns
				XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING.
				Note: If the document does not have a DOCTYPE declaration at all,
				then startDoctypeDeclHandler and endDoctypeDeclHandler will not
				be called, despite an external subset being parsed.
				Note: If XML_DTD is not defined when Expat is compiled, returns
				XML_ERROR_FEATURE_REQUIRES_XML_DTD.
				*/
				XML_Error use_foreign_dtd(bool v);
				/*
					Sets the base to be used for resolving relative URIs in system
					identifiers in declarations.Resolving relative identifiers is
					left to the application : this value will be passed through as the
					base argument to the XML_ExternalEntityRefHandler,
					XML_NotationDeclHandler and XML_UnparsedEntityDeclHandler.
				*/
				void base(const std::string& v);
				std::string base() const;

				/* This is equivalent to supplying an encoding argument to
				XML_ParserCreate. On success XML_SetEncoding returns non-zero,
				zero otherwise.
				Note: Calling XML_SetEncoding after XML_Parse or XML_ParseBuffer
				has no effect and returns XML_STATUS_ERROR.
				*/
				XML_Status set_encoding(const std::string& encoding);

				XML_Status parse(const char* input, std::size_t len);
				XML_Status parse(const std::string& input);
				XML_Status feed(const char* input, std::size_t len, bool is_final);
				/* Stops parsing, causing XML_Parse() or XML_ParseBuffer() to return.
				Must be called from within a call-back handler, except when aborting
				(resumable = 0) an already suspended parser. Some call-backs may
				still follow because they would otherwise get lost. Examples:
				- endElementHandler() for empty elements when stopped in
				startElementHandler(),
				- endNameSpaceDeclHandler() when stopped in endElementHandler(),
				and possibly others.

				Can be called from most handlers, including DTD related call-backs,
				except when parsing an external parameter entity and resumable != 0.
				Returns XML_STATUS_OK when successful, XML_STATUS_ERROR otherwise.
				Possible error codes:
				- XML_ERROR_SUSPENDED: when suspending an already suspended parser.
				- XML_ERROR_FINISHED: when the parser has already finished.
				- XML_ERROR_SUSPEND_PE: when suspending while parsing an external PE.

				When resumable != 0 (true) then parsing is suspended, that is,
				XML_Parse() and XML_ParseBuffer() return XML_STATUS_SUSPENDED.
				Otherwise, parsing is aborted, that is, XML_Parse() and XML_ParseBuffer()
				return XML_STATUS_ERROR with error code XML_ERROR_ABORTED.

				*Note*:
				This will be applied to the current parser instance only, that is, if
				there is a parent parser then it will continue parsing when the
				externalEntityRefHandler() returns. It is up to the implementation of
				the externalEntityRefHandler() to call XML_StopParser() on the parent
				parser (recursively), if one wants to stop parsing altogether.

				When suspended, parsing can be resumed by calling XML_ResumeParser().
				*/
				XML_Status stop(bool is_resumable);
				/* Resumes parsing after it has been suspended with XML_StopParser().
				Must not be called from within a handler call-back. Returns same
				status codes as XML_Parse() or XML_ParseBuffer().
				Additional error code XML_ERROR_NOT_SUSPENDED possible.

				*Note*:
				This must be called on the most deeply nested child parser instance
				first, and on its parent parser only after the child parser has finished,
				to be applied recursively until the document entity's parser is restarted.
				That is, the parent parser will not resume by itself and it is up to the
				application to call XML_ResumeParser() on it at the appropriate moment.
				*/
				XML_Status resume();

				void status(XML_Parsing& parser_status, bool& is_final) const;
				/* Controls parsing of parameter entities (including the external DTD
				subset). If parsing of parameter entities is enabled, then
				references to external parameter entities (including the external
				DTD subset) will be passed to the handler set with
				XML_SetExternalEntityRefHandler.  The context passed will be 0.

				Unlike external general entities, external parameter entities can
				only be parsed synchronously.  If the external parameter entity is
				to be parsed, it must be parsed during the call to the external
				entity ref handler: the complete sequence of
				XML_ExternalEntityParserCreate, XML_Parse/XML_ParseBuffer and
				XML_ParserFree calls must be made during this call.  After
				XML_ExternalEntityParserCreate has been called to create the parser
				for the external parameter entity (context must be 0 for this
				call), it is illegal to make any calls on the old parser until
				XML_ParserFree has been called on the newly created parser.
				If the library has been compiled without support for parameter
				entity parsing (ie without XML_DTD being defined), then
				XML_SetParamEntityParsing will return 0 if parsing of parameter
				entities is requested; otherwise it will return non-zero.
				Note: If XML_SetParamEntityParsing is called after XML_Parse or
				XML_ParseBuffer, then it has no effect and will always return 0.
				*/
				int param_entity_policy(XML_ParamEntityParsing policy);

				XML_Error error() const;
				static std::string describe_error(XML_Error e);
				unsigned line() const;
				unsigned column() const;
				unsigned byte_count() const;

				static void version(int& major, int& minor, int& micro);
			protected:
				explicit parser_base(XML_Parser other);

				XML_Parser _create_external_entity(const std::string& context, 
					const std::string& encoding);

				void _user_data(void* obj);
			protected:
				XML_Parser m_parser;
			private:
				bool       m_is_owner;
			};

			
		} // ns detail
	} // ns xml
} // ns bsw

std::ostream& operator << (std::ostream& os, XML_Error e);

#endif
