//
// Created by igor on 1/11/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_URI_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_URI_HH_

#include <assets/assets_export.h>
#include <vector>
#include <string>

namespace neutrino::assets::datastore {
	class path;

	class ASSETS_EXPORT uri
		/// A Uniform Resource Identifier, as specified in RFC 3986.
		///
		/// The URI class provides methods for building URIs from their
		/// parts, as well as for splitting URIs into their parts.
		/// Furthermore, the class provides methods for resolving
		/// relative URIs against base URIs.
		///
		/// The class automatically performs a few normalizations on
		/// all URIs and URI parts passed to it:
		///   * scheme identifiers are converted to lower case
		///   * percent-encoded characters are decoded (except for the query string)
		///   * optionally, dot segments are removed from paths (see normalize())
		///
		/// Note that dealing with query strings requires some precautions, as, internally,
		/// query strings are stored in percent-encoded form, while all other parts of the URI
		/// are stored in decoded form. While parsing query strings from properly encoded URLs
		/// generally works, explicitly setting query strings with set_query() or extracting
		/// query strings with get_query() may lead to ambiguities. See the descriptions of
		/// set_query(), set_raw_query(), get_query() and get_raw_query() for more information.
	{
	 public:
		using query_params_t = std::vector<std::pair<std::string, std::string>>;

		uri ();
		/// Creates an empty URI.

		explicit uri (const std::string& uri_string);
		/// Parses an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.

		explicit uri (const char* uri_string);
		/// Parses an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.

		uri (std::string  scheme, const std::string& pathEtc);
		/// Creates an URI from its parts.

		uri (std::string  scheme, const std::string& authority, const std::string& pathEtc);
		/// Creates an URI from its parts.

		uri (std::string  scheme, const std::string& authority, std::string  pth, std::string  query);
		/// Creates an URI from its parts.

		uri (std::string  scheme, const std::string& authority, std::string  pth, std::string  query, std::string  fragment);
		/// Creates an URI from its parts.

		uri (const uri& other);
		/// Copy constructor. Creates an URI from another one.

		uri (uri&& other) noexcept;
		/// Move constructor.

		uri (const uri& base_uri, const std::string& relative_uri);
		/// Creates an URI from a base URI and a relative URI, according to
		/// the algorithm in section 5.2 of RFC 3986.

		explicit uri (const path& pth);
		/// Creates a URI from a path.
		///
		/// The path will be made absolute, and a file:// URI
		/// will be built from it.

		~uri ();
		/// Destroys the URI.

		uri& operator= (const uri& other);
		/// Assignment operator.

		uri& operator= (uri&& other) noexcept;
		/// Move assignment.

		uri& operator= (const std::string& other);
		/// Parses and assigns an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.

		uri& operator= (const char* other);
		/// Parses and assigns an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.

		void swap (uri& other);
		/// Swaps the URI with another one.

		void clear ();
		/// Clears all parts of the URI.

		[[nodiscard]] std::string to_string () const;
		/// Returns a string representation of the URI.
		///
		/// Characters in the path, query and fragment parts will be
		/// percent-encoded as necessary.

		[[nodiscard]] const std::string& get_scheme () const;
		/// Returns the scheme part of the URI.

		void set_scheme (const std::string& scheme);
		/// Sets the scheme part of the URI. The given scheme
		/// is converted to lower-case.
		///
		/// A list of registered URI schemes can be found
		/// at <http://www.iana.org/assignments/uri-schemes>.

		[[nodiscard]] const std::string& get_user_info () const;
		/// Returns the user-info part of the URI.

		void set_user_info (const std::string& userInfo);
		/// Sets the user-info part of the URI.

		[[nodiscard]] const std::string& get_host () const;
		/// Returns the host part of the URI.

		void setHost (const std::string& host);
		/// Sets the host part of the URI.

		[[nodiscard]] unsigned short get_port () const;
		/// Returns the port number part of the URI.
		///
		/// If no port number (0) has been specified, the
		/// well-known port number (e.g., 80 for http) for
		/// the given scheme is returned if it is known.
		/// Otherwise, 0 is returned.

		void set_port (unsigned short port);
		/// Sets the port number part of the URI.

		[[nodiscard]] unsigned short get_specified_port () const;
		/// Returns the port number part of the URI.
		///
		/// If no explicit port number has been specified,
		/// returns 0.

		[[nodiscard]] std::string get_authority () const;
		/// Returns the authority part (userInfo, host and port)
		/// of the URI.
		///
		/// If the port number is a well-known port
		/// number for the given scheme (e.g., 80 for http), it
		/// is not included in the authority.

		void set_authority (const std::string& authority);
		/// Parses the given authority part for the URI and sets
		/// the user-info, host, port components accordingly.

		[[nodiscard]] const std::string& get_path () const;
		/// Returns the decoded path part of the URI.

		void set_path (const std::string& path);
		/// Sets the path part of the URI.

		[[nodiscard]] std::string get_query () const;
		/// Returns the decoded query part of the URI.
		///
		/// Note that encoded ampersand characters ('&', "%26")
		/// will be decoded, which could cause ambiguities if the query
		/// string contains multiple parameters and a parameter name
		/// or value contains an ampersand as well.
		/// In such a case it's better to use get_raw_query() or
		/// get_query_parameters().

		void set_query (const std::string& query);
		/// Sets the query part of the URI.
		///
		/// The query string will be percent-encoded. If the query
		/// already contains percent-encoded characters, these
		/// will be double-encoded, which is probably not what's
		/// intended by the caller. Furthermore, ampersand ('&')
		/// characters in the query will not be encoded. This could
		/// lead to ambiguity issues if the query string contains multiple
		/// name-value parameters separated by ampersand, and if any
		/// name or value also contains an ampersand. In such a
		/// case, it's better to use set_raw_query() with a properly
		/// percent-encoded query string, or use add_query_parameter()
		/// or set_query_parameters(), which take care of appropriate
		/// percent encoding of parameter names and values.

		void add_query_parameter (const std::string& param, const std::string& val = "");
		/// Adds "param=val" to the query; "param" may not be empty.
		/// If val is empty, only '=' is appended to the parameter.
		///
		/// In addition to regular encoding, function also encodes '&' and '=',
		/// if found in param or val.

		[[nodiscard]] const std::string& get_raw_query () const;
		/// Returns the query string in raw form, which usually
		/// means percent encoded.

		void set_raw_query (const std::string& query);
		/// Sets the query part of the URI.
		///
		/// The given query string must be properly percent-encoded.

		[[nodiscard]] query_params_t get_query_parameters () const;
		/// Returns the decoded query string parameters as a vector
		/// of name-value pairs.

		void set_query_parameters (const query_params_t& params);
		/// Sets the query part of the URI from a vector
		/// of query parameters.
		///
		/// Calls add_query_parameter() for each parameter name and value.

		[[nodiscard]] const std::string& get_fragment () const;
		/// Returns the fragment part of the URI.

		void set_fragment (const std::string& fragment);
		/// Sets the fragment part of the URI.

		void set_path_etc (const std::string& pathEtc);
		/// Sets the path, query and fragment parts of the URI.

		[[nodiscard]] std::string get_path_etc () const;
		/// Returns the encoded path, query and fragment parts of the URI.

		[[nodiscard]] std::string get_path_and_query () const;
		/// Returns the encoded path and query parts of the URI.

		void resolve (const std::string& relative_uri);
		/// Resolves the given relative URI against the base URI.
		/// See section 5.2 of RFC 3986 for the algorithm used.

		void resolve (const uri& relative_uri);
		/// Resolves the given relative URI against the base URI.
		/// See section 5.2 of RFC 3986 for the algorithm used.

		[[nodiscard]] bool is_relative () const;
		/// Returns true if the URI is a relative reference, false otherwise.
		///
		/// A relative reference does not contain a scheme identifier.
		/// Relative references are usually resolved against an absolute
		/// base reference.

		[[nodiscard]] bool empty () const;
		/// Returns true if the URI is empty, false otherwise.

		bool operator== (const uri& other) const;
		/// Returns true if both URIs are identical, false otherwise.
		///
		/// Two URIs are identical if their scheme, authority,
		/// path, query and fragment part are identical.

		bool operator== (const std::string& other) const;
		/// Parses the given URI and returns true if both URIs are identical,
		/// false otherwise.

		bool operator!= (const uri& other) const;
		/// Returns true if both URIs are identical, false otherwise.

		bool operator!= (const std::string& other) const;
		/// Parses the given URI and returns true if both URIs are identical,
		/// false otherwise.

		void normalize ();
		/// Normalizes the URI by removing all but leading . and .. segments from the path.
		///
		/// If the first path segment in a relative path contains a colon (:),
		/// such as in a Windows path containing a drive letter, a dot segment (./)
		/// is prepended in accordance with section 3.3 of RFC 3986.

		void get_path_segments (std::vector<std::string>& segments);
		/// Places the single path segments (delimited by slashes) into the
		/// given vector.

		static void encode (const std::string& str, const std::string& reserved, std::string& encoded_str);
		/// URI-encodes the given string by escaping reserved and non-ASCII
		/// characters. The encoded string is appended to encodedStr.

		static void decode (const std::string& str, std::string& decoded_str, bool plus_as_space = false);
		/// URI-decodes the given string by replacing percent-encoded
		/// characters with the actual character. The decoded string
		/// is appended to decodedStr.
		///
		/// When plusAsSpace is true, non-encoded plus signs in the query are decoded as spaces.
		/// (http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1)

	 protected:
		[[nodiscard]] bool equals (const uri& uri) const;
		/// Returns true if both uri's are equivalent.

		[[nodiscard]] bool is_well_known_port () const;
		/// Returns true if the URI's port number is a well-known one
		/// (for example, 80, if the scheme is http).

		[[nodiscard]] unsigned short get_well_known_port () const;
		/// Returns the well-known port number for the URI's scheme,
		/// or 0 if the port number is not known.

		void parse (const std::string& uri);
		/// Parses and assigns an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.

		void parse_authority (std::string::const_iterator& it, const std::string::const_iterator& end);
		/// Parses and sets the user-info, host and port from the given data.

		void parse_host_and_port (std::string::const_iterator& it, const std::string::const_iterator& end);
		/// Parses and sets the host and port from the given data.

		void parse_path (std::string::const_iterator& it, const std::string::const_iterator& end);
		/// Parses and sets the path from the given data.

		void parse_path_etc (std::string::const_iterator& it, const std::string::const_iterator& end);
		/// Parses and sets the path, query and fragment from the given data.

		void parse_query (std::string::const_iterator& it, const std::string::const_iterator& end);
		/// Parses and sets the query from the given data.

		void parse_fragment (std::string::const_iterator& it, const std::string::const_iterator& end);
		/// Parses and sets the fragment from the given data.

		void merge_path (const std::string& pth);
		/// Appends a path to the URI's path.

		void remove_dot_segments (bool remove_leading = true);
		/// Removes all dot segments from the path.

		static void get_path_segments (const std::string& pth, std::vector<std::string>& segments);
		/// Places the single path segments (delimited by slashes) into the
		/// given vector.

		void build_path (const std::vector<std::string>& segments, bool leading_slash, bool trailing_slash);
		/// Builds the path from the given segments.

		static const std::string RESERVED_PATH;
		static const std::string RESERVED_QUERY;
		static const std::string RESERVED_QUERY_PARAM;
		static const std::string RESERVED_FRAGMENT;
		static const std::string ILLEGAL;

	 private:
		std::string m_scheme;
		std::string m_user_info;
		std::string m_host;
		unsigned short m_port;
		std::string m_path;
		std::string m_query;
		std::string m_fragment;
	};

//
// inlines
//
	inline const std::string& uri::get_scheme () const {
		return m_scheme;
	}

	inline const std::string& uri::get_user_info () const {
		return m_user_info;
	}

	inline const std::string& uri::get_host () const {
		return m_host;
	}

	inline const std::string& uri::get_path () const {
		return m_path;
	}

	inline const std::string& uri::get_raw_query () const {
		return m_query;
	}

	inline const std::string& uri::get_fragment () const {
		return m_fragment;
	}

	inline unsigned short uri::get_specified_port () const {
		return m_port;
	}

	inline void swap (uri& u1, uri& u2) {
		u1.swap (u2);
	}

}

#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_URI_HH_
