//
// Created by igor on 1/11/24.
//

#include <assets/datastore/uri.hh>
#include <assets/datastore/path.hh>
#include <bsw/strings/number_formatter.hh>
#include <bsw/strings/number_parser.hh>
#include <bsw/strings/string_utils.hh>
#include <bsw/exception.hh>
#include <utility>

namespace neutrino::assets::datastore {
	const std::string uri::RESERVED_PATH = "?#";
	const std::string uri::RESERVED_QUERY = "?#/:;+@";
	const std::string uri::RESERVED_QUERY_PARAM = "?#/:;+@&=";
	const std::string uri::RESERVED_FRAGMENT;
	const std::string uri::ILLEGAL = "%<>{}|\\\"^`!*'()$,[]";

	uri::uri ()
		:
		m_port (0) {
	}

	uri::uri (const std::string& uri_string)
		:
		m_port (0) {
		parse (uri_string);
	}

	uri::uri (const char* uri_string)
		:
		m_port (0) {
		parse (std::string (uri_string));
	}

	uri::uri (std::string  scheme, const std::string& pathEtc)
		:
		m_scheme (std::move(scheme)),
		m_port (0) {
		bsw::to_lower_in_place (m_scheme);

		std::string::const_iterator beg = pathEtc.begin ();
		std::string::const_iterator end = pathEtc.end ();
		parse_path_etc (beg, end);
	}

	uri::uri (std::string  scheme, const std::string& authority, const std::string& pathEtc)
		:
		m_scheme (std::move(scheme)) {
		bsw::to_lower_in_place (m_scheme);
		std::string::const_iterator beg = authority.begin ();
		std::string::const_iterator end = authority.end ();
		parse_authority (beg, end);
		beg = pathEtc.begin ();
		end = pathEtc.end ();
		parse_path_etc (beg, end);
	}

	uri::uri (std::string  scheme, const std::string& authority, std::string  pth, std::string  query)
		:
		m_scheme (std::move(scheme)),
		m_path (std::move(pth)),
		m_query (std::move(query)),
		m_port(0) {
		bsw::to_lower_in_place (m_scheme);
		std::string::const_iterator beg = authority.begin ();
		std::string::const_iterator end = authority.end ();
		parse_authority (beg, end);
	}

	uri::uri (std::string  scheme, const std::string& authority, std::string  pth, std::string  query, std::string  fragment)
		:
		m_scheme (std::move(scheme)),
		m_path (std::move(pth)),
		m_query (std::move(query)),
		m_fragment (std::move(fragment)) {
		bsw::to_lower_in_place (m_scheme);
		std::string::const_iterator beg = authority.begin ();
		std::string::const_iterator end = authority.end ();
		parse_authority (beg, end);
	}

	uri::uri (const uri& other)
		:
		m_scheme (other.m_scheme),
		m_user_info (other.m_user_info),
		m_host (other.m_host),
		m_port (other.m_port),
		m_path (other.m_path),
		m_query (other.m_query),
		m_fragment (other.m_fragment) {
	}

	uri::uri (uri&& other) noexcept
		:
		m_scheme (std::move (other.m_scheme)),
		m_user_info (std::move (other.m_user_info)),
		m_host (std::move (other.m_host)),
		m_port (other.m_port),
		m_path (std::move (other.m_path)),
		m_query (std::move (other.m_query)),
		m_fragment (std::move (other.m_fragment)) {
	}

	uri::uri (const uri& base_uri, const std::string& relative_uri)
		:
		m_scheme (base_uri.m_scheme),
		m_user_info (base_uri.m_user_info),
		m_host (base_uri.m_host),
		m_port (base_uri.m_port),
		m_path (base_uri.m_path),
		m_query (base_uri.m_query),
		m_fragment (base_uri.m_fragment) {
		resolve (relative_uri);
	}

	uri::uri (const path& pth)
		:
		m_scheme ("file"),
		m_port (0) {
		const path& absolutePath (pth);
		ENFORCE(pth.is_absolute ());
		// absolutePath.makeAbsolute();
		m_path = absolutePath.to_string (path::PATH_UNIX);
	}

	uri::~uri () = default;

	uri& uri::operator= (const uri& other) {
		if (&other != this) {
			m_scheme = other.m_scheme;
			m_user_info = other.m_user_info;
			m_host = other.m_host;
			m_port = other.m_port;
			m_path = other.m_path;
			m_query = other.m_query;
			m_fragment = other.m_fragment;
		}
		return *this;
	}

	uri& uri::operator= (uri&& other) noexcept {
		m_scheme = std::move (other.m_scheme);
		m_user_info = std::move (other.m_user_info);
		m_host = std::move (other.m_host);
		m_port = other.m_port;
		m_path = std::move (other.m_path);
		m_query = std::move (other.m_query);
		m_fragment = std::move (other.m_fragment);

		return *this;
	}

	uri& uri::operator= (const std::string& other) {
		clear ();
		parse (other);
		return *this;
	}

	uri& uri::operator= (const char* other) {
		clear ();
		parse (std::string (other));
		return *this;
	}

	void uri::swap (uri& other) {
		std::swap (m_scheme, other.m_scheme);
		std::swap (m_user_info, other.m_user_info);
		std::swap (m_host, other.m_host);
		std::swap (m_port, other.m_port);
		std::swap (m_path, other.m_path);
		std::swap (m_query, other.m_query);
		std::swap (m_fragment, other.m_fragment);
	}

	void uri::clear () {
		m_scheme.clear ();
		m_user_info.clear ();
		m_host.clear ();
		m_port = 0;
		m_path.clear ();
		m_query.clear ();
		m_fragment.clear ();
	}

	std::string uri::to_string () const {
		std::string uri;
		if (is_relative ()) {
			encode (m_path, RESERVED_PATH, uri);
		} else {
			uri = m_scheme;
			uri += ':';
			std::string auth = get_authority ();
			if (!auth.empty () || m_scheme == "file") {
				uri.append ("//");
				uri.append (auth);
			}
			if (!m_path.empty ()) {
				if (!auth.empty () && m_path[0] != '/') {
					uri += '/';
				}
				encode (m_path, RESERVED_PATH, uri);
			} else if (!m_query.empty () || !m_fragment.empty ()) {
				uri += '/';
			}
		}
		if (!m_query.empty ()) {
			uri += '?';
			uri.append (m_query);
		}
		if (!m_fragment.empty ()) {
			uri += '#';
			encode (m_fragment, RESERVED_FRAGMENT, uri);
		}
		return uri;
	}

	void uri::set_scheme (const std::string& scheme) {
		m_scheme = scheme;
		bsw::to_lower_in_place (m_scheme);
	}

	void uri::set_user_info (const std::string& userInfo) {
		m_user_info.clear ();
		decode (userInfo, m_user_info);
	}

	void uri::setHost (const std::string& host) {
		m_host = host;
	}

	unsigned short uri::get_port () const {
		if (m_port == 0) {
			return get_well_known_port ();
		} else {
			return m_port;
		}
	}

	void uri::set_port (unsigned short port) {
		m_port = port;
	}

	std::string uri::get_authority () const {
		std::string auth;
		if (!m_user_info.empty ()) {
			auth.append (m_user_info);
			auth += '@';
		}
		if (m_host.find (':') != std::string::npos) {
			auth += '[';
			auth += m_host;
			auth += ']';
		} else { auth.append (m_host); }
		if (m_port && !is_well_known_port ()) {
			auth += ':';
			bsw::number_formatter::append (auth, m_port);
		}
		return auth;
	}

	void uri::set_authority (const std::string& authority) {
		m_user_info.clear ();
		m_host.clear ();
		m_port = 0;
		std::string::const_iterator beg = authority.begin ();
		std::string::const_iterator end = authority.end ();
		parse_authority (beg, end);
	}

	void uri::set_path (const std::string& path) {
		m_path.clear ();
		decode (path, m_path);
	}

	void uri::set_raw_query (const std::string& query) {
		m_query = query;
	}

	void uri::set_query (const std::string& query) {
		m_query.clear ();
		encode (query, RESERVED_QUERY, m_query);
	}

	void uri::add_query_parameter (const std::string& param, const std::string& val) {
		if (!m_query.empty ()) { m_query += '&'; }
		encode (param, RESERVED_QUERY_PARAM, m_query);
		m_query += '=';
		encode (val, RESERVED_QUERY_PARAM, m_query);
	}

	std::string uri::get_query () const {
		std::string query;
		decode (m_query, query);
		return query;
	}

	uri::query_params_t uri::get_query_parameters () const {
		query_params_t result;
		std::string::const_iterator it (m_query.begin ());
		std::string::const_iterator end (m_query.end ());
		while (it != end) {
			std::string name;
			std::string value;
			while (it != end && *it != '=' && *it != '&') {
				if (*it == '+') {
					name += ' ';
				} else {
					name += *it;
				}
				++it;
			}
			if (it != end && *it == '=') {
				++it;
				while (it != end && *it != '&') {
					if (*it == '+') {
						value += ' ';
					} else {
						value += *it;
					}
					++it;
				}
			}
			std::string decodedName;
			std::string decodedValue;
			uri::decode (name, decodedName);
			uri::decode (value, decodedValue);
			result.push_back (std::make_pair (decodedName, decodedValue));
			if (it != end && *it == '&') { ++it; }
		}
		return result;
	}

	void uri::set_query_parameters (const query_params_t& params) {
		m_query.clear ();
		for (const auto& p : params) {
			add_query_parameter (p.first, p.second);
		}
	}

	void uri::set_fragment (const std::string& fragment) {
		m_fragment.clear ();
		decode (fragment, m_fragment);
	}

	void uri::set_path_etc (const std::string& pathEtc) {
		m_path.clear ();
		m_query.clear ();
		m_fragment.clear ();
		std::string::const_iterator beg = pathEtc.begin ();
		std::string::const_iterator end = pathEtc.end ();
		parse_path_etc (beg, end);
	}

	std::string uri::get_path_etc () const {
		std::string pathEtc;
		encode (m_path, RESERVED_PATH, pathEtc);
		if (!m_query.empty ()) {
			pathEtc += '?';
			pathEtc += m_query;
		}
		if (!m_fragment.empty ()) {
			pathEtc += '#';
			encode (m_fragment, RESERVED_FRAGMENT, pathEtc);
		}
		return pathEtc;
	}

	std::string uri::get_path_and_query () const {
		std::string pathAndQuery;
		encode (m_path, RESERVED_PATH, pathAndQuery);
		if (!m_query.empty ()) {
			pathAndQuery += '?';
			pathAndQuery += m_query;
		}
		return pathAndQuery;
	}

	void uri::resolve (const std::string& relative_uri) {
		uri parsedURI (relative_uri);
		resolve (parsedURI);
	}

	void uri::resolve (const uri& relative_uri) {
		if (!relative_uri.m_scheme.empty ()) {
			m_scheme = relative_uri.m_scheme;
			m_user_info = relative_uri.m_user_info;
			m_host = relative_uri.m_host;
			m_port = relative_uri.m_port;
			m_path = relative_uri.m_path;
			m_query = relative_uri.m_query;
			remove_dot_segments ();
		} else {
			if (!relative_uri.m_host.empty ()) {
				m_user_info = relative_uri.m_user_info;
				m_host = relative_uri.m_host;
				m_port = relative_uri.m_port;
				m_path = relative_uri.m_path;
				m_query = relative_uri.m_query;
				remove_dot_segments ();
			} else {
				if (relative_uri.m_path.empty ()) {
					if (!relative_uri.m_query.empty ()) {
						m_query = relative_uri.m_query;
					}
				} else {
					if (relative_uri.m_path[0] == '/') {
						m_path = relative_uri.m_path;
						remove_dot_segments ();
					} else {
						merge_path (relative_uri.m_path);
					}
					m_query = relative_uri.m_query;
				}
			}
		}
		m_fragment = relative_uri.m_fragment;
	}

	bool uri::is_relative () const {
		return m_scheme.empty ();
	}

	bool uri::empty () const {
		return m_scheme.empty () && m_host.empty () && m_path.empty () && m_query.empty () && m_fragment.empty ();
	}

	bool uri::operator== (const uri& other) const {
		return equals (other);
	}

	bool uri::operator== (const std::string& other) const {
		uri parsedURI (other);
		return equals (parsedURI);
	}

	bool uri::operator!= (const uri& other) const {
		return !equals (other);
	}

	bool uri::operator!= (const std::string& other) const {
		uri parsedURI (other);
		return !equals (parsedURI);
	}

	bool uri::equals (const uri& uri) const {
		return m_scheme == uri.m_scheme
			   && m_user_info == uri.m_user_info
			   && m_host == uri.m_host
			   && get_port () == uri.get_port ()
			   && m_path == uri.m_path
			   && m_query == uri.m_query
			   && m_fragment == uri.m_fragment;
	}

	void uri::normalize () {
		remove_dot_segments (!is_relative ());
	}

	void uri::remove_dot_segments (bool remove_leading) {
		if (m_path.empty ()) { return; }

		bool leadingSlash = *(m_path.begin ()) == '/';
		bool trailingSlash = *(m_path.rbegin ()) == '/';
		std::vector<std::string> segments;
		std::vector<std::string> normalizedSegments;
		get_path_segments (segments);
		for (const auto& s : segments) {
			if (s == "..") {
				if (!normalizedSegments.empty ()) {
					if (normalizedSegments.back () == "..") {
						normalizedSegments.push_back (s);
					} else {
						normalizedSegments.pop_back ();
					}
				} else if (!remove_leading) {
					normalizedSegments.push_back (s);
				}
			} else if (s != ".") {
				normalizedSegments.push_back (s);
			}
		}
		build_path (normalizedSegments, leadingSlash, trailingSlash);
	}

	void uri::get_path_segments (std::vector<std::string>& segments) {
		get_path_segments (m_path, segments);
	}

	void uri::get_path_segments (const std::string& pth, std::vector<std::string>& segments) {
		std::string::const_iterator it = pth.begin ();
		std::string::const_iterator end = pth.end ();
		std::string seg;
		while (it != end) {
			if (*it == '/') {
				if (!seg.empty ()) {
					segments.push_back (seg);
					seg.clear ();
				}
			} else { seg += *it; }
			++it;
		}
		if (!seg.empty ()) {
			segments.push_back (seg);
		}
	}

	void uri::encode (const std::string& str, const std::string& reserved, std::string& encoded_str) {
		for (auto c : str) {
			if ((c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c >= '0' && c <= '9') ||
				c == '-' || c == '_' ||
				c == '.' || c == '~') {
				encoded_str += c;
			} else if (c <= 0x20 || c >= 0x7F || ILLEGAL.find (c) != std::string::npos
					   || reserved.find (c) != std::string::npos) {
				encoded_str += '%';
				encoded_str += bsw::number_formatter::format_hex ((unsigned)(unsigned char)c, 2);
			} else { encoded_str += c; }
		}
	}

	void uri::decode (const std::string& str, std::string& decoded_str, bool plus_as_space) {
		bool inQuery = false;
		std::string::const_iterator it = str.begin ();
		std::string::const_iterator end = str.end ();
		while (it != end) {
			char c = *it++;
			if (c == '?') { inQuery = true; }
			// spaces may be encoded as plus signs in the query
			if (inQuery && plus_as_space && c == '+') { c = ' '; }
			else if (c == '%') {
				if (it == end) RAISE_EX("URI encoding: no hex digit following percent sign", str);
				char hi = *it++;
				if (it == end) RAISE_EX("URI encoding: two hex digits must follow percent sign", str);
				char lo = *it++;
				if (hi >= '0' && hi <= '9') {
					c = hi - '0';
				} else if (hi >= 'A' && hi <= 'F') {
					c = hi - 'A' + 10;
				} else if (hi >= 'a' && hi <= 'f') {
					c = hi - 'a' + 10;
				} else
					RAISE_EX("URI encoding: not a hex digit");
				c *= 16;
				if (lo >= '0' && lo <= '9') {
					c += lo - '0';
				} else if (lo >= 'A' && lo <= 'F') {
					c += lo - 'A' + 10;
				} else if (lo >= 'a' && lo <= 'f') {
					c += lo - 'a' + 10;
				} else
					RAISE_EX("URI encoding: not a hex digit");
			}
			decoded_str += c;
		}
	}

	bool uri::is_well_known_port () const {
		return m_port == get_well_known_port ();
	}

	unsigned short uri::get_well_known_port () const {
		if (m_scheme == "ftp") {
			return 21;
		} else if (m_scheme == "ssh") {
			return 22;
		} else if (m_scheme == "telnet") {
			return 23;
		} else if (m_scheme == "smtp") {
			return 25;
		} else if (m_scheme == "dns") {
			return 53;
		} else if (m_scheme == "http" || m_scheme == "ws") {
			return 80;
		} else if (m_scheme == "nntp") {
			return 119;
		} else if (m_scheme == "imap") {
			return 143;
		} else if (m_scheme == "ldap") {
			return 389;
		} else if (m_scheme == "https" || m_scheme == "wss") {
			return 443;
		} else if (m_scheme == "smtps") {
			return 465;
		} else if (m_scheme == "rtsp") {
			return 554;
		} else if (m_scheme == "ldaps") {
			return 636;
		} else if (m_scheme == "dnss") {
			return 853;
		} else if (m_scheme == "imaps") {
			return 993;
		} else if (m_scheme == "sip") {
			return 5060;
		} else if (m_scheme == "sips") {
			return 5061;
		} else if (m_scheme == "xmpp") {
			return 5222;
		} else {
			return 0;
		}
	}

	void uri::parse (const std::string& uri) {
		std::string::const_iterator it = uri.begin ();
		std::string::const_iterator end = uri.end ();
		if (it == end) { return; }
		if (*it != '/' && *it != '.' && *it != '?' && *it != '#') {
			std::string scheme;
			while (it != end && *it != ':' && *it != '?' && *it != '#' && *it != '/') { scheme += *it++; }
			if (it != end && *it == ':') {
				++it;
				if (it == end) RAISE_EX("URI scheme must be followed by authority or path", uri);
				set_scheme (scheme);
				if (*it == '/') {
					++it;
					if (it != end && *it == '/') {
						++it;
						parse_authority (it, end);
					} else { --it; }
				}
				parse_path_etc (it, end);
			} else {
				it = uri.begin ();
				parse_path_etc (it, end);
			}
		} else { parse_path_etc (it, end); }
	}

	void uri::parse_authority (std::string::const_iterator& it, const std::string::const_iterator& end) {
		std::string userInfo;
		std::string part;
		while (it != end && *it != '/' && *it != '?' && *it != '#') {
			if (*it == '@') {
				userInfo = part;
				part.clear ();
			} else { part += *it; }
			++it;
		}
		std::string::const_iterator pbeg = part.begin ();
		std::string::const_iterator pend = part.end ();
		parse_host_and_port (pbeg, pend);
		m_user_info = userInfo;
	}

	void uri::parse_host_and_port (std::string::const_iterator& it, const std::string::const_iterator& end) {
		if (it == end) { return; }
		std::string host;
		if (*it == '[') {
			// IPv6 address
			++it;
			while (it != end && *it != ']') { host += *it++; }
			if (it == end) RAISE_EX("unterminated IPv6 address");
			++it;
		} else {
			while (it != end && *it != ':') { host += *it++; }
		}
		if (it != end && *it == ':') {
			++it;
			std::string port;
			while (it != end) { port += *it++; }
			if (!port.empty ()) {
				int nport = 0;
				if (bsw::number_parser::try_parse (port, nport) && nport > 0 && nport < 65536) {
					m_port = (unsigned short)nport;
				} else
					RAISE_EX("bad or invalid port number", port);
			} else { m_port = 0; }
		} else { m_port = 0; }
		m_host = host;
		bsw::to_lower_in_place (m_host);
	}

	void uri::parse_path (std::string::const_iterator& it, const std::string::const_iterator& end) {
		std::string path;
		while (it != end && *it != '?' && *it != '#') { path += *it++; }
		decode (path, m_path);
	}

	void uri::parse_path_etc (std::string::const_iterator& it, const std::string::const_iterator& end) {
		if (it == end) { return; }
		if (*it != '?' && *it != '#') {
			parse_path (it, end);
		}
		if (it != end && *it == '?') {
			++it;
			parse_query (it, end);
		}
		if (it != end && *it == '#') {
			++it;
			parse_fragment (it, end);
		}
	}

	void uri::parse_query (std::string::const_iterator& it, const std::string::const_iterator& end) {
		m_query.clear ();
		while (it != end && *it != '#') { m_query += *it++; }
	}

	void uri::parse_fragment (std::string::const_iterator& it, const std::string::const_iterator& end) {
		std::string fragment;
		while (it != end) { fragment += *it++; }
		decode (fragment, m_fragment);
	}

	void uri::merge_path (const std::string& pth) {
		std::vector<std::string> segments;
		std::vector<std::string> normalizedSegments;
		bool addLeadingSlash = false;
		if (!m_path.empty ()) {
			get_path_segments (segments);
			bool endsWithSlash = *(m_path.rbegin ()) == '/';
			if (!endsWithSlash && !segments.empty ()) {
				segments.pop_back ();
			}
			addLeadingSlash = m_path[0] == '/';
		}
		get_path_segments (pth, segments);
		addLeadingSlash = addLeadingSlash || (!pth.empty () && pth[0] == '/');
		bool hasTrailingSlash = (!pth.empty () && *(pth.rbegin ()) == '/');
		bool addTrailingSlash = false;
		for (const auto& s : segments) {
			if (s == "..") {
				addTrailingSlash = true;
				if (!normalizedSegments.empty ()) {
					normalizedSegments.pop_back ();
				}
			} else if (s != ".") {
				addTrailingSlash = false;
				normalizedSegments.push_back (s);
			} else { addTrailingSlash = true; }
		}
		build_path (normalizedSegments, addLeadingSlash, hasTrailingSlash || addTrailingSlash);
	}

	void uri::build_path (const std::vector<std::string>& segments, bool leading_slash, bool trailing_slash) {
		m_path.clear ();
		bool first = true;
		for (const auto& s : segments) {
			if (first) {
				first = false;
				if (leading_slash) {
					m_path += '/';
				} else if (m_scheme.empty () && s.find (':') != std::string::npos) {
					m_path.append ("./");
				}
			} else { m_path += '/'; }
			m_path.append (s);
		}
		if (trailing_slash) {
			m_path += '/';
		}
	}

}