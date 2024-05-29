#ifndef ARES_BSW_XML_HANDLER_IFACE_HH
#define ARES_BSW_XML_HANDLER_IFACE_HH

#include <string>

#include "xml/epp/xml_handler.hh"
#include "xml/epp/expat_inc.hh"
#include <bsw/exception.hh>
#include "bsw/macros.hh"

#define DECLARE_HAS_METHOD(NAME, RET_TYPE, ...)\
template <class T> struct PPCAT(has_, NAME)\
{\
        typedef char yes[1];\
        typedef yes no[2];\
        template <typename U, U u> struct reallyHas;\
        template <typename C> static yes& test(reallyHas<RET_TYPE (C::*)(__VA_ARGS__), &C::NAME>*);\
        template <typename C> static yes& test(reallyHas<RET_TYPE(C::*)(__VA_ARGS__) const, &C::NAME>*);\
        template <typename> static no& test(...);\
        static const bool value = sizeof(test<T>(0)) == sizeof(yes);\
}

namespace bsw
{
	namespace xml
	{
		
		/**
		 * XML declaration handler
		 * void xml_declaration (const std::string& version, const std::string& encoding, standalone_t);
		 */

		 /**
		  * TEXT declaration handler
		  * void text_declaration (const std::string& encoding, standalone_t);
		  */

		/**
		 * Start element handler
		 * void start_element (const std::string& name, const attributes& atts);
		 */

		/**
		 * End element handler
		 * void end_element (const std::string& name);
		 */

		/**
		 * Character data handler
		 * void character_data (const char* data, std::size_t length);
		 */

		/**
		 * Processing instruction
		 * void processing_instruction (const std::string& target, const std::string& data);
		 */

		 /**
		 * Comment handler
		 * void comment (const std::string& s);
		 */

		 /**
		 * Start CDATA
		 * void start_cdata ();
		 */

		 /**
		 * End CDATA
		 * void end_cdata ();
		 */

		 /* The Attlist declaration handler is called for *each* attribute. So
		 a single Attlist declaration with multiple attributes declared will
		 generate multiple calls to this handler. The "default" parameter
		 may be NULL in the case of the "#IMPLIED" or "#REQUIRED"
		 keyword. The "isrequired" parameter will be true and the default
		 value will be NULL in the case of "#REQUIRED". If "isrequired" is
		 true and default is non-NULL, then this is a "#FIXED" default.

		 void attribute (const std::string& elname,
                                    const std::string& attname,
                                    const std::string& att_type,
                                    const std::string& dflt,
                                    bool              isrequired);
		 */

		 /* This is called for any characters in the XML document for which
		 there is no applicable handler.  This includes both characters that
		 are part of markup which is of a kind that is not reported
		 (comments, markup declarations), or characters that are part of a
		 construct which could be reported but for which no handler has been
		 supplied. The characters are passed exactly as they were in the XML
		 document except that they will be encoded in UTF-8 or UTF-16.
		 Line boundaries are not normalized. Note that a byte order mark
		 character is not passed to the default handler. There are no
		 guarantees about how characters are divided between calls to the
		 default handler: for example, a comment might be split between
		 multiple calls.
		 
		 void default_handler (const char* s, std::size_t len);
		 */

		 /* This is called for the start of the DOCTYPE declaration, before
			any DTD or internal subset is parsed.

			void start_doctype (const std::string& doctypeName,
                                            const std::string& sysid,
                                            const std::string& pubid,
                                            bool has_internal_subset);
		 */

		 /* This is called for the start of the DOCTYPE declaration when the
		 closing > is encountered, but after processing any external
		 subset.
			void end_doctype ();
		 */

		 /* This is called for entity declarations. The is_parameter_entity
		 argument will be non-zero if the entity is a parameter entity, zero
		 otherwise.

		 For internal entities (<!ENTITY foo "bar">), value will
		 be non-NULL and systemId, publicID, and notationName will be NULL.
		 The value string is NOT nul-terminated; the length is provided in
		 the value_length argument. Since it is legal to have zero-length
		 values, do not use this argument to test for internal entities.

		 For external entities, value will be NULL and systemId will be
		 non-NULL. The publicId argument will be NULL unless a public
		 identifier was provided. The notationName argument will have a
		 non-NULL value only for unparsed entity declarations.

		 Note that is_parameter_entity can't be changed to XML_Bool, since
		 that would break binary compatibility.
		 
		void entity (
			const std::string& entityName,
			bool is_parameter_entity,
			const char* value,
			std::size_t value_length,
			const std::string& base,
			const std::string& systemId,
			const std::string& publicId,
			const std::string& notationName);
			*/

			/* This is called for a declaration of notation.  The base argument is
			whatever was set by XML_SetBase. The notationName will never be
			NULL.  The other arguments can be.
			void notation (const std::string& notationName, const std::string& base,
						   const std::string& systemId, const std::string& publicId);
			*/

			/* When namespace processing is enabled, these are called once for
			each namespace declaration. The call to the start and end element
			handlers occur between the calls to the start and end namespace
			declaration handlers. For an xmlns attribute, prefix will be
			NULL.  For an xmlns="" attribute, uri will be NULL.

			void start_namespace (const std::string& prefix, const std::string& uri);
			void end_namespace (const std::string& prefix);
			*/

			/* This is called in two situations:
			1) An entity reference is encountered for which no declaration
			has been read *and* this is not an error.
			2) An internal entity reference is read, but not expanded, because
			XML_SetDefaultHandler has been called.
			Note: skipped parameter entities in declarations and skipped general
			entities in attribute values cannot be reported, because
			the event would be out of sync with the reporting of the
			declarations or attribute values

			void skipped_entity (const std::string& entityName, bool is_parameter_entity);
			*/

			/* This is called if the document is not standalone, that is, it has an
			external subset or a reference to a parameter entity, but does not
			have standalone="yes". If this handler returns XML_STATUS_ERROR,
			then processing will not continue, and the parser will return a
			XML_ERROR_NOT_STANDALONE error.
			If parameter entity parsing is enabled, then in addition to the
			conditions above this handler will only be called if the referenced
			entity was actually read.

			bool not_standalone ();
			*/



		namespace detail
		{
			DECLARE_HAS_METHOD(xml_declaration, void, const std::string&, const std::string&, standalone_t);
			DECLARE_HAS_METHOD(text_declaration, void, const std::string&, standalone_t);
			

			template <typename Obj>
			struct XmlDeclHandler
			{
				template <typename T>
				static typename std::enable_if <has_text_declaration<T>::value, void>::type
					do_text_declaration(T& obj, const std::string& encoding, standalone_t s)
				{
					obj.text_declaration(encoding, s);
				}

				template <typename T>
				static typename std::enable_if <!has_text_declaration<T>::value, void>::type
					do_text_declaration(T&, const std::string&, standalone_t)
				{
				}

				template <typename T>
				static typename std::enable_if <has_xml_declaration<T>::value, void>::type
					do_xml_declaration(T& obj, const std::string& version, const std::string& encoding, standalone_t s)
				{
					obj.xml_declaration(version, encoding, s);
				}

				template <typename T>
				static typename std::enable_if <!has_xml_declaration<T>::value, void>::type
					do_xml_declaration(T&, const std::string&, standalone_t)
				{
				}

				static void handle(void *userData, const XML_Char *version, const XML_Char *encoding, int standalone)
				{
					static auto to_enum = [](int s) -> standalone_t {
						switch (s)
						{
						case 0:
							return standalone_t::no;
						case 1:
							return standalone_t::yes;
						default:
							return standalone_t::unknown;
						}
					};
					if (version)
					{
						do_xml_declaration(*reinterpret_cast<Obj*>(userData), std::string(version), std::string(encoding), to_enum(standalone));
					}
					else
					{
						do_text_declaration(*reinterpret_cast<Obj*>(userData), std::string(encoding), to_enum(standalone));
					}
				}
			};

			template <typename Obj>
			typename std::enable_if <has_text_declaration<Obj>::value || has_xml_declaration<Obj>::value, void>::type
			register_XmlDeclHandler(XML_Parser parser)
			{
					XML_SetXmlDeclHandler(parser, &XmlDeclHandler<Obj>::handle);
			}

			template <typename Obj>
			typename std::enable_if <!has_text_declaration<Obj>::value && !has_xml_declaration<Obj>::value, void>::type
				register_XmlDeclHandler(XML_Parser parser)
			{
			}
			// ===============================================================================
#define DECLARE_APPLY_METHOD(MTH, RET_TYPE, ...)													\
	template <typename T>																\
	static typename std::enable_if <!PPCAT(has_,MTH)<T>::value, RET_TYPE>::type		\
	apply(T&, ##__VA_ARGS__)																\
	{}																					\
	template <typename T>																\
	static typename std::enable_if <PPCAT(has_,MTH)<T>::value, RET_TYPE>::type		\
	apply(T& obj, ##__VA_ARGS__)															\

#define DECLARE_REGISTER_METHOD(IFACE, MTH)														\
			template <typename Obj>																\
			typename std::enable_if <PPCAT(has_,MTH)<Obj>::value, void>::type				\
				PPCAT(register_, IFACE)(XML_Parser parser)								\
			{																					\
				PPCAT(XML_Set, IFACE) (parser, &IFACE<Obj>::handle);						\
			}																					\
			template <typename Obj>																\
			typename std::enable_if <!PPCAT(has_,MTH)<Obj>::value, void>::type			\
				PPCAT(register_, IFACE)(XML_Parser parser)								\
			{}

#define START_INTERFACE_DECL_TYPE(IFACE, MTH, RET_TYPE, ...)													\
		DECLARE_HAS_METHOD(MTH, RET_TYPE, ##__VA_ARGS__);												\
		template <typename Obj>																	\
		struct IFACE																			\
		{																						\
			DECLARE_APPLY_METHOD(MTH, RET_TYPE, ##__VA_ARGS__)												

#define START_INTERFACE_DECL(IFACE, MTH, ...) START_INTERFACE_DECL_TYPE(IFACE, MTH, void, ##__VA_ARGS__)

#define END_INTERFACE_DECL(IFACE, MTH)															\
		};																						\
		DECLARE_REGISTER_METHOD(IFACE, MTH)

			// =====================================================================================
			
			START_INTERFACE_DECL(StartElementHandler, start_element, const std::string& name, const bsw::xml::attributes& atts)
			{
				obj.start_element(name, atts);
			}
			static void handle(void *userData, const XML_Char* name, const XML_Char **atts)
			{
				apply(*reinterpret_cast<Obj*>(userData), name, attributes(atts));
			}
			END_INTERFACE_DECL(StartElementHandler, start_element)
			// ===============================================================================
			START_INTERFACE_DECL(EndElementHandler, end_element, const std::string& name)
			{
				obj.end_element (name);
			}
			static void handle(void *userData, const XML_Char* name)
			{
				apply(*reinterpret_cast<Obj*>(userData), name);
			}
			END_INTERFACE_DECL(EndElementHandler, end_element)
			// ==========================================================================
			START_INTERFACE_DECL(CharacterDataHandler, character_data, const char* str, std::size_t len)
			{
				obj.character_data(str, len);
			}
			static void handle(void *userData, const XML_Char* s, int len)
			{
				ENFORCE(len >= 0);
				apply(*reinterpret_cast<Obj*>(userData), s, static_cast<std::size_t>(len));
			}
			END_INTERFACE_DECL(CharacterDataHandler, character_data)
			// ==========================================================================
			START_INTERFACE_DECL(ProcessingInstructionHandler, processing_instruction, const std::string& target, const std::string& data)
			{
				obj.processing_instruction(target, data);
			}
			static void handle(void *userData, const XML_Char* target, const XML_Char* data)
			{
				apply(*reinterpret_cast<Obj*>(userData), target, data);
			}
			END_INTERFACE_DECL(ProcessingInstructionHandler, processing_instruction)
			// ==========================================================================
			START_INTERFACE_DECL(CommentHandler, comment, const std::string& s)
			{
				obj.comment(s);
			}
			static void handle(void *userData, const XML_Char* s)
			{
				apply(*reinterpret_cast<Obj*>(userData), s);
			}
			END_INTERFACE_DECL(CommentHandler, comment)
			// ==========================================================================
			START_INTERFACE_DECL(StartCdataSectionHandler, start_cdata)
			{
				obj.start_cdata ();
			}
			static void handle(void *userData)
			{
				apply(*reinterpret_cast<Obj*>(userData));
			}
			END_INTERFACE_DECL(StartCdataSectionHandler, start_cdata)
			// ==========================================================================
			START_INTERFACE_DECL(EndCdataSectionHandler, end_cdata)
			{
				obj.end_cdata();
			}
			static void handle(void *userData)
			{
				apply(*reinterpret_cast<Obj*>(userData));
			}
			END_INTERFACE_DECL(EndCdataSectionHandler, end_cdata)
			// =======================================================================================
			START_INTERFACE_DECL(AttlistDeclHandler, attribute, 
				const std::string& elname, const std::string& attname,
				const std::string& att_type, const std::string& dflt,
				bool              isrequired)
			{
				obj.attribute(elname, attname, att_type, dflt, isrequired);
			}
			static void handle(void *userData, const XML_Char  *elname,
				const XML_Char  *attname,
				const XML_Char  *att_type,
				const XML_Char  *dflt,
				int              isrequired)
			{
				apply(*reinterpret_cast<Obj*>(userData), elname, attname, att_type, dflt, isrequired != 0);
			}
			END_INTERFACE_DECL(AttlistDeclHandler, attribute)
			// =====================================================================================
			START_INTERFACE_DECL(DefaultHandler, default_handler, const char* str, std::size_t len)
			{
				obj.default_handler(str, len);
			}
			static void handle(void *userData, const XML_Char* s, int len)
			{
				ENFORCE(len >= 0);
				apply(*reinterpret_cast<Obj*>(userData), s, static_cast<std::size_t>(len));
			}
			END_INTERFACE_DECL(DefaultHandler, default_handler)
			// =====================================================================================
			START_INTERFACE_DECL(StartDoctypeDeclHandler, start_doctype, const std::string& doctypeName,
				const std::string& sysid,
				const std::string& pubid,
				bool has_internal_subset)
			{
				obj.start_doctype(doctypeName, sysid, pubid, has_internal_subset);
			}
			static void handle(void *userData, const XML_Char *doctypeName,
				const XML_Char *sysid,
				const XML_Char *pubid,
				int has_internal_subset)
			{
				apply(*reinterpret_cast<Obj*>(userData), doctypeName, sysid, pubid, has_internal_subset != 0);
			}
			END_INTERFACE_DECL(StartDoctypeDeclHandler, start_doctype)
			// =====================================================================================
			START_INTERFACE_DECL(EndDoctypeDeclHandler, end_doctype)
			{
				obj.end_doctype();
			}
			static void handle(void *userData)
			{
				apply(*reinterpret_cast<Obj*>(userData));
			}
			END_INTERFACE_DECL(EndDoctypeDeclHandler, end_doctype)
			// ======================================================================================
			START_INTERFACE_DECL(EntityDeclHandler, entity,
				const std::string& entityName,
				bool is_parameter_entity,
				const char* value,
				std::size_t value_length,
				const std::string& base,
				const std::string& systemId,
				const std::string& publicId,
				const std::string& notationName)
			{
				obj.entity(entityName, is_parameter_entity, value, value_length, base, systemId, publicId, notationName);
			}
			static void handle(void *userData, const XML_Char *entityName,
				int is_parameter_entity,
				const XML_Char *value,
				int value_length,
				const XML_Char *base,
				const XML_Char *systemId,
				const XML_Char *publicId,
				const XML_Char *notationName)
			{
				ENFORCE(value_length >= 0);
				apply(*reinterpret_cast<Obj*>(userData), 
					entityName, 
					is_parameter_entity != 0,
					value,
					static_cast <std::size_t>(value_length),
					base,
					systemId,
					publicId,
					notationName
					);
			}
			END_INTERFACE_DECL(EntityDeclHandler, entity)
			// ==============================================================================
			START_INTERFACE_DECL(NotationDeclHandler, notation, const std::string& notationName, const std::string& base,
				const std::string& systemId, const std::string& publicId)
			{
				obj.notation(notationName, base, systemId, publicId);
			}
			static void handle(void *userData,
				const XML_Char *notationName,
				const XML_Char *base,
				const XML_Char *systemId,
				const XML_Char *publicId)
			{
				apply(*reinterpret_cast<Obj*>(userData), notationName, base, systemId, publicId);
			}
			END_INTERFACE_DECL(NotationDeclHandler, notation)
			// ================================================================================
			START_INTERFACE_DECL(StartNamespaceDeclHandler, start_namespace, const std::string& prefix, const std::string& uri)
			{
				obj.start_namespace(prefix, uri);
			}
			static void handle(void *userData, const XML_Char *prefix, const XML_Char *uri)
			{
				apply(*reinterpret_cast<Obj*>(userData), prefix, uri);
			}
			END_INTERFACE_DECL(StartNamespaceDeclHandler, start_namespace)
			// ===============================================================================
			START_INTERFACE_DECL(EndNamespaceDeclHandler, end_namespace, const std::string& prefix)
			{
				obj.end_namespace(prefix);
			}
			static void handle(void *userData, const XML_Char *prefix)
			{
				apply(*reinterpret_cast<Obj*>(userData), prefix);
			}
			END_INTERFACE_DECL(EndNamespaceDeclHandler, end_namespace)
			// ==================================================================================
			START_INTERFACE_DECL(SkippedEntityHandler, skipped_entity, const std::string& entityName, bool is_parameter_entity)
			{
				obj.skipped_entity(entityName, is_parameter_entity);
			}
			static void handle(void *userData, const XML_Char *entityName, int is_parameter_entity)
			{
				apply(*reinterpret_cast<Obj*>(userData), entityName, is_parameter_entity != 0);
			}
			END_INTERFACE_DECL(SkippedEntityHandler, skipped_entity)
			// =====================================================================================
			START_INTERFACE_DECL_TYPE(NotStandaloneHandler, not_standalone, bool)
			{
				return obj.not_standalone();
			}
			static int handle(void *userData)
			{
				return apply(*reinterpret_cast<Obj*>(userData)) ? XML_STATUS_OK : XML_STATUS_ERROR;
			}
			END_INTERFACE_DECL(NotStandaloneHandler, not_standalone)
			// =====================================================================================
		} // ns detail
	} // ns xml
} // namespace bsw

#endif
