//
// Created by igor on 1/11/24.
//

#include <doctest/doctest.h>
#include <assets/datastore/uri.hh>
#include <assets/datastore/path.hh>

using namespace neutrino::assets::datastore;

TEST_SUITE("uri test") {
	TEST_CASE("test construction") {
		uri uri1;
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());

		uri1.set_scheme ("ftp");
		REQUIRE (uri1.get_scheme () == "ftp");
		REQUIRE (uri1.get_port () == 21);

		uri1.set_scheme ("HTTP");
		REQUIRE (uri1.get_scheme () == "http");

		uri1.set_authority ("www.appinf.com");
		REQUIRE (uri1.get_authority () == "www.appinf.com");
		REQUIRE (uri1.get_port () == 80);
		REQUIRE (uri1.get_specified_port () == 0);

		uri1.set_authority ("user@services.appinf.com:8000");
		REQUIRE (uri1.get_user_info () == "user");
		REQUIRE (uri1.get_host () == "services.appinf.com");
		REQUIRE (uri1.get_port () == 8000);
		REQUIRE (uri1.get_specified_port () == 8000);

		uri1.set_path ("/index.html");
		REQUIRE (uri1.get_path () == "/index.html");

		uri1.set_path ("/file%20with%20spaces.html");
		REQUIRE (uri1.get_path () == "/file with spaces.html");

		uri1.set_path_etc ("/query.cgi?query=foo");
		REQUIRE (uri1.get_path () == "/query.cgi");
		REQUIRE (uri1.get_query () == "query=foo");
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.get_path_etc () == "/query.cgi?query=foo");
		REQUIRE (uri1.get_path_and_query () == "/query.cgi?query=foo");

		uri1.set_path_etc ("/query.cgi?query=bar#frag");
		REQUIRE (uri1.get_path () == "/query.cgi");
		REQUIRE (uri1.get_query () == "query=bar");
		REQUIRE (uri1.get_fragment () == "frag");
		REQUIRE (uri1.get_path_etc () == "/query.cgi?query=bar#frag");
		REQUIRE (uri1.get_path_and_query () == "/query.cgi?query=bar");

		uri1.set_query ("query=test");
		REQUIRE (uri1.get_query () == "query=test");

		uri1.set_fragment ("result");
		REQUIRE (uri1.get_fragment () == "result");

		uri uri2 ("file", "/home/guenter/foo.bar");
		REQUIRE (uri2.get_scheme () == "file");
		REQUIRE (uri2.get_path () == "/home/guenter/foo.bar");

		uri uri3 ("http", "www.appinf.com", "/index.html");
		REQUIRE (uri3.get_scheme () == "http");
		REQUIRE (uri3.get_authority () == "www.appinf.com");
		REQUIRE (uri3.get_path () == "/index.html");

		uri uri4 ("http", "www.appinf.com:8000", "/index.html");
		REQUIRE (uri4.get_scheme () == "http");
		REQUIRE (uri4.get_authority () == "www.appinf.com:8000");
		REQUIRE (uri4.get_path () == "/index.html");

		uri uri5 ("http", "user@www.appinf.com:8000", "/index.html");
		REQUIRE (uri5.get_scheme () == "http");
		REQUIRE (uri5.get_user_info () == "user");
		REQUIRE (uri5.get_host () == "www.appinf.com");
		REQUIRE (uri5.get_port () == 8000);
		REQUIRE (uri5.get_authority () == "user@www.appinf.com:8000");
		REQUIRE (uri5.get_path () == "/index.html");

		uri uri6 ("http", "user@www.appinf.com:80", "/index.html");
		REQUIRE (uri6.get_scheme () == "http");
		REQUIRE (uri6.get_user_info () == "user");
		REQUIRE (uri6.get_host () == "www.appinf.com");
		REQUIRE (uri6.get_port () == 80);
		REQUIRE (uri6.get_specified_port () == 80);
		REQUIRE (uri6.get_authority () == "user@www.appinf.com");
		REQUIRE (uri6.get_path () == "/index.html");

		uri uri7 ("http", "user@www.appinf.com:", "/index.html");
		REQUIRE (uri7.get_scheme () == "http");
		REQUIRE (uri7.get_user_info () == "user");
		REQUIRE (uri7.get_host () == "www.appinf.com");
		REQUIRE (uri7.get_port () == 80);
		REQUIRE (uri7.get_specified_port () == 0);
		REQUIRE (uri7.get_authority () == "user@www.appinf.com");
		REQUIRE (uri7.get_path () == "/index.html");

		uri uri8 ("http", "www.appinf.com", "/index.html", "query=test");
		REQUIRE (uri8.get_scheme () == "http");
		REQUIRE (uri8.get_authority () == "www.appinf.com");
		REQUIRE (uri8.get_path () == "/index.html");
		REQUIRE (uri8.get_query () == "query=test");

		uri uri9 ("http", "www.appinf.com", "/index.html", "query=test", "fragment");
		REQUIRE (uri9.get_scheme () == "http");
		REQUIRE (uri9.get_authority () == "www.appinf.com");
		REQUIRE (uri9.get_path () == "/index.html");
		REQUIRE (uri9.get_path_etc () == "/index.html?query=test#fragment");
		REQUIRE (uri9.get_query () == "query=test");
		REQUIRE (uri9.get_fragment () == "fragment");

		uri9.clear ();
		REQUIRE (uri9.get_scheme ().empty ());
		REQUIRE (uri9.get_authority ().empty ());
		REQUIRE (uri9.get_user_info ().empty ());
		REQUIRE (uri9.get_host ().empty ());
		REQUIRE (uri9.get_port () == 0);
		REQUIRE (uri9.get_path ().empty ());
		REQUIRE (uri9.get_query ().empty ());
		REQUIRE (uri9.get_fragment ().empty ());

		uri uri10 ("ldap", "[2001:db8::7]", "/c=GB?objectClass?one");
		REQUIRE (uri10.get_scheme () == "ldap");
		REQUIRE (uri10.get_user_info ().empty ());
		REQUIRE (uri10.get_host () == "2001:db8::7");
		REQUIRE (uri10.get_port () == 389);
		REQUIRE (uri10.get_specified_port () == 0);
		REQUIRE (uri10.get_authority () == "[2001:db8::7]");
		REQUIRE (uri10.get_path_etc () == "/c=GB?objectClass?one");

		uri uri11 ("http", "www.appinf.com", "/index.html?query=test#fragment");
		REQUIRE (uri11.get_scheme () == "http");
		REQUIRE (uri11.get_authority () == "www.appinf.com");
		REQUIRE (uri11.get_path () == "/index.html");
		REQUIRE (uri11.get_path_etc () == "/index.html?query=test#fragment");
		REQUIRE (uri11.get_query () == "query=test");
		REQUIRE (uri11.get_fragment () == "fragment");
	}

	TEST_CASE("test parse") {
		uri uri1 ("http://www.appinf.com");
		REQUIRE (uri1.get_scheme () == "http");
		REQUIRE (uri1.get_authority () == "www.appinf.com");
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (!uri1.is_relative ());

		uri1 = "http://www.appinf.com/";
		REQUIRE (uri1.get_scheme () == "http");
		REQUIRE (uri1.get_authority () == "www.appinf.com");
		REQUIRE (uri1.get_path () == "/");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (!uri1.is_relative ());

		uri1 = "ftp://anonymous@ftp.appinf.com/pub/";
		REQUIRE (uri1.get_scheme () == "ftp");
		REQUIRE (uri1.get_user_info () == "anonymous");
		REQUIRE (uri1.get_host () == "ftp.appinf.com");
		REQUIRE (uri1.get_port () == 21);
		REQUIRE (uri1.get_authority () == "anonymous@ftp.appinf.com");
		REQUIRE (uri1.get_path () == "/pub/");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (!uri1.is_relative ());
		REQUIRE (!uri1.is_relative ());

		uri1 = "https://www.appinf.com/index.html#top";
		REQUIRE (uri1.get_scheme () == "https");
		REQUIRE (uri1.get_host () == "www.appinf.com");
		REQUIRE (uri1.get_port () == 443);
		REQUIRE (uri1.get_path () == "/index.html");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment () == "top");
		REQUIRE (!uri1.is_relative ());

		uri1 = "http://www.appinf.com/search.cgi?keyword=test&scope=all";
		REQUIRE (uri1.get_scheme () == "http");
		REQUIRE (uri1.get_host () == "www.appinf.com");
		REQUIRE (uri1.get_port () == 80);
		REQUIRE (uri1.get_path () == "/search.cgi");
		REQUIRE (uri1.get_query () == "keyword=test&scope=all");
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (!uri1.is_relative ());

		uri1 = "http://www.appinf.com/search.cgi?keyword=test&scope=all#result";
		REQUIRE (uri1.get_scheme () == "http");
		REQUIRE (uri1.get_host () == "www.appinf.com");
		REQUIRE (uri1.get_port () == 80);
		REQUIRE (uri1.get_path () == "/search.cgi");
		REQUIRE (uri1.get_query () == "keyword=test&scope=all");
		REQUIRE (uri1.get_fragment () == "result");
		REQUIRE (!uri1.is_relative ());

		uri1 = "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result";
		REQUIRE (uri1.get_scheme () == "http");
		REQUIRE (uri1.get_host () == "www.appinf.com");
		REQUIRE (uri1.get_port () == 80);
		REQUIRE (uri1.get_path () == "/search.cgi");
		REQUIRE (uri1.get_query () == "keyword=test encoded&scope=all");
		REQUIRE (uri1.get_fragment () == "result");
		REQUIRE (!uri1.is_relative ());

		uri1 = "ldap://[2001:db8::7]/c=GB?objectClass?one";
		REQUIRE (uri1.get_scheme () == "ldap");
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host () == "2001:db8::7");
		REQUIRE (uri1.get_port () == 389);
		REQUIRE (uri1.get_authority () == "[2001:db8::7]");
		REQUIRE (uri1.get_path () == "/c=GB");
		REQUIRE (uri1.get_query () == "objectClass?one");
		REQUIRE (uri1.get_fragment ().empty ());

		uri1 = "mailto:John.Doe@example.com";
		REQUIRE (uri1.get_scheme () == "mailto");
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_path () == "John.Doe@example.com");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());

		uri1 = "tel:+1-816-555-1212";
		REQUIRE (uri1.get_scheme () == "tel");
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_path () == "+1-816-555-1212");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());

		uri1 = "telnet://192.0.2.16:80";
		REQUIRE (uri1.get_scheme () == "telnet");
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host () == "192.0.2.16");
		REQUIRE (uri1.get_port () == 80);
		REQUIRE (uri1.get_authority () == "192.0.2.16:80");
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());

		uri1 = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
		REQUIRE (uri1.get_scheme () == "urn");
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_path () == "oasis:names:specification:docbook:dtd:xml:4.1.2");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());

		uri1 = "";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.empty ());

		// relative references

		uri1 = "/foo/bar";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "/foo/bar");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.is_relative ());

		uri1 = "./foo/bar";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "./foo/bar");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.is_relative ());

		uri1 = "../foo/bar";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "../foo/bar");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.is_relative ());

		uri1 = "index.html";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "index.html");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.is_relative ());

		uri1 = "index.html#frag";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "index.html");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment () == "frag");
		REQUIRE (uri1.is_relative ());

		uri1 = "?query=test";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query () == "query=test");
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.is_relative ());

		uri1 = "?query=test#frag";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query () == "query=test");
		REQUIRE (uri1.get_fragment () == "frag");
		REQUIRE (uri1.is_relative ());

		uri1 = "#frag";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment () == "frag");
		REQUIRE (uri1.is_relative ());

		uri1 = "#";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path ().empty ());
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.is_relative ());

		uri1 = "file:///a/b/c";
		REQUIRE (uri1.get_scheme () == "file");
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "/a/b/c");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (!uri1.is_relative ());

		uri1 = "file://localhost/a/b/c";
		REQUIRE (uri1.get_scheme () == "file");
		REQUIRE (uri1.get_authority () == "localhost");
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host () == "localhost");
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "/a/b/c");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (!uri1.is_relative ());

		uri1 = "file:///c:/Windows/system32/";
		REQUIRE (uri1.get_scheme () == "file");
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "/c:/Windows/system32/");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (!uri1.is_relative ());

		uri1 = "./c:/Windows/system32/";
		REQUIRE (uri1.get_scheme ().empty ());
		REQUIRE (uri1.get_authority ().empty ());
		REQUIRE (uri1.get_user_info ().empty ());
		REQUIRE (uri1.get_host ().empty ());
		REQUIRE (uri1.get_port () == 0);
		REQUIRE (uri1.get_path () == "./c:/Windows/system32/");
		REQUIRE (uri1.get_query ().empty ());
		REQUIRE (uri1.get_fragment ().empty ());
		REQUIRE (uri1.is_relative ());

		uri1 = "ws://www.appinf.com/ws";
		REQUIRE (uri1.get_scheme () == "ws");
		REQUIRE (uri1.get_port () == 80);

		uri1 = "wss://www.appinf.com/ws";
		REQUIRE (uri1.get_scheme () == "wss");
		REQUIRE (uri1.get_port () == 443);
	}

	TEST_CASE("test toString") {
		uri uri1 ("http://www.appinf.com");
		REQUIRE (uri1.to_string () == "http://www.appinf.com");

		uri1 = "http://www.appinf.com/";
		REQUIRE (uri1.to_string () == "http://www.appinf.com/");

		uri1 = "ftp://anonymous@ftp.appinf.com/pub/";
		REQUIRE (uri1.to_string () == "ftp://anonymous@ftp.appinf.com/pub/");

		uri1 = "https://www.appinf.com/index.html#top";
		REQUIRE (uri1.to_string () == "https://www.appinf.com/index.html#top");

		uri1 = "http://www.appinf.com/search.cgi?keyword=test&scope=all";
		REQUIRE (uri1.to_string () == "http://www.appinf.com/search.cgi?keyword=test&scope=all");

		uri1 = "http://www.appinf.com/search.cgi?keyword=test&scope=all#result";
		REQUIRE (uri1.to_string () == "http://www.appinf.com/search.cgi?keyword=test&scope=all#result");

		uri1 = "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result";
		REQUIRE (uri1.to_string () == "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");

		uri1 = "ldap://[2001:db8::7]/c=GB?objectClass?one";
		REQUIRE (uri1.to_string () == "ldap://[2001:db8::7]/c=GB?objectClass?one");

		uri1 = "mailto:John.Doe@example.com";
		REQUIRE (uri1.to_string () == "mailto:John.Doe@example.com");

		uri1 = "tel:+1-816-555-1212";
		REQUIRE (uri1.to_string () == "tel:+1-816-555-1212");

		uri1 = "telnet://192.0.2.16:80";
		REQUIRE (uri1.to_string () == "telnet://192.0.2.16:80");

		uri1 = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
		REQUIRE (uri1.to_string () == "urn:oasis:names:specification:docbook:dtd:xml:4.1.2");

		uri1 = "";
		REQUIRE (uri1.to_string () == "");

		// relative references

		uri1 = "/foo/bar";
		REQUIRE (uri1.to_string () == "/foo/bar");

		uri1 = "./foo/bar";
		REQUIRE (uri1.to_string () == "./foo/bar");

		uri1 = "../foo/bar";
		REQUIRE (uri1.to_string () == "../foo/bar");

		uri1 = "//foo/bar";
		REQUIRE (uri1.to_string () == "//foo/bar");

		uri1 = "index.html";
		REQUIRE (uri1.to_string () == "index.html");

		uri1 = "index.html#frag";
		REQUIRE (uri1.to_string () == "index.html#frag");

		uri1 = "?query=test";
		REQUIRE (uri1.to_string () == "?query=test");

		uri1 = "?query=test#frag";
		REQUIRE (uri1.to_string () == "?query=test#frag");

		uri1 = "#frag";
		REQUIRE (uri1.to_string () == "#frag");

		uri1 = "#";
		REQUIRE (uri1.to_string () == "");

		uri1 = "file:///a/b/c";
		REQUIRE (uri1.to_string () == "file:///a/b/c");

		uri1 = "file://localhost/a/b/c";
		REQUIRE (uri1.to_string () == "file://localhost/a/b/c");

		uri1 = "file:///c:/Windows/system32/";
		REQUIRE (uri1.to_string () == "file:///c:/Windows/system32/");

		uri1 = "./c:/Windows/system32/";
		REQUIRE (uri1.to_string () == "./c:/Windows/system32/");

		uri1 = "http://www.appinf.com";
		uri1.set_raw_query ("query=test");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/?query=test");
	}

	TEST_CASE("test compare") {
		uri uri1 ("http://www.appinf.com");
		uri uri2 ("HTTP://www.appinf.com:80");
		REQUIRE (uri1 == uri2);
		REQUIRE (uri1 == "http://www.appinf.com:");
		REQUIRE (uri1 != "http://www.google.com");

		uri1 = "/foo/bar";
		REQUIRE (uri1 == "/foo/bar");
		REQUIRE (uri1 != "/foo/baz");

		uri1 = "?query";
		REQUIRE (uri1 == "?query");
		REQUIRE (uri1 != "?query2");

		uri1 = "#frag";
		REQUIRE (uri1 == "#frag");
		REQUIRE (uri1 != "#frag2");

		uri1 = "/index.html#frag";
		REQUIRE (uri1 == "/index.html#frag");
		REQUIRE (uri1 != "/index.html");
	}

	TEST_CASE("test normalize") {
		uri uri1 ("http://www.appinf.com");
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com");

		uri1 = "http://www.appinf.com/";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/");

		uri1 = "http://www.appinf.com/foo/bar/./index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/foo/bar/index.html");

		uri1 = "http://www.appinf.com/foo/bar/../index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/foo/index.html");

		uri1 = "http://www.appinf.com/foo/./bar/../index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/foo/index.html");

		uri1 = "http://www.appinf.com/foo/./bar/../index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/foo/index.html");

		uri1 = "http://www.appinf.com/foo/bar/../../index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/index.html");

		uri1 = "http://www.appinf.com/foo/bar/../../../index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/index.html");

		uri1 = "http://www.appinf.com/foo/bar/.././../index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/index.html");

		uri1 = "http://www.appinf.com/./foo/bar/index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/foo/bar/index.html");

		uri1 = "http://www.appinf.com/../foo/bar/index.html";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/foo/bar/index.html");

		uri1 = "http://www.appinf.com/../foo/bar/";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/foo/bar/");

		uri1 = "http://www.appinf.com/../foo/../";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "http://www.appinf.com/");

		uri1 = "file:///c:/Windows/system32/";
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "file:///c:/Windows/system32/");

		uri1.clear ();
		uri1.set_path ("c:/windows/system32/");
		uri1.normalize ();
		REQUIRE (uri1.to_string () == "./c:/windows/system32/");
	}

	TEST_CASE("test resolve") {
		uri uri1 ("http://www.appinf.com");

		uri1.resolve ("/index.html");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/index.html");

		uri1.resolve ("#frag");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/index.html#frag");

		uri1 = "http://www.appinf.com/html";
		uri1.resolve ("../images/foo.gif");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/images/foo.gif");

		uri1 = "http://www.appinf.com/html/";
		uri1.resolve (".");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/html/");

		uri1 = "http://www.appinf.com/html/";
		uri1.resolve (".");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/html/");

		uri1 = "http://www.appinf.com/html/";
		uri1.resolve ("..");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/");

		uri1 = "http://www.appinf.com/html/";
		uri1.resolve ("index.html");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/html/index.html");

		uri1 = "http://www.appinf.com/html/";
		uri1.resolve ("/index.html");
		REQUIRE (uri1.to_string () == "http://www.appinf.com/index.html");

		uri1 = "/a/b/c/d/e";
		uri1.resolve ("./../../f/./g");
		REQUIRE (uri1.to_string () == "/a/b/f/g");

		uri1 = "/a/b/../c/";
		uri1.resolve ("../d");
		REQUIRE (uri1.to_string () == "/a/d");

		uri1 = "/a/b/../c/";
		uri1.resolve ("../d/");
		REQUIRE (uri1.to_string () == "/a/d/");

		uri1 = "/a/b/c/";
		uri1.resolve ("../../../../d/");
		REQUIRE (uri1.to_string () == "/d/");

		uri1 = "/a/b/c/";
		uri1.resolve ("%2e%2e/%2e%2e/%2e%2e/../d/");
		REQUIRE (uri1.to_string () == "/d/");

		uri1 = "/a/b/c/";
		uri1.resolve ("");
		REQUIRE (uri1.to_string () == "/a/b/c/");

		uri1 = "/a/b/c/";
		uri1.resolve ("/d/");
		REQUIRE (uri1.to_string () == "/d/");

		uri1 = "/a/b/c";
		uri1.resolve ("");
		REQUIRE (uri1.to_string () == "/a/b/c");

		uri1 = "/a/b/c";
		uri1.resolve ("?query=test");
		REQUIRE (uri1.to_string () == "/a/b/c?query=test");

		uri1 = "/a/b/c";
		uri1.resolve ("#frag");
		REQUIRE (uri1.to_string () == "/a/b/c#frag");

		uri1 = "http://www.appinf.com/html/";
		uri1.resolve ("http://www.google.com/");
		REQUIRE (uri1.to_string () == "http://www.google.com/");

		uri1 = "http://www.appinf.com/";
		uri uri2 (uri1, "index.html");
		REQUIRE (uri2.to_string () == "http://www.appinf.com/index.html");

		uri1 = "index.html";
		uri uri3 (uri1, "search.html");
		REQUIRE (uri3.to_string () == "search.html");
	}

	TEST_CASE("test swap") {
		uri uri1 ("http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");
		uri uri2 ("mailto:John.Doe@example.com");

		uri1.swap (uri2);
		REQUIRE (uri1.to_string () == "mailto:John.Doe@example.com");
		REQUIRE (uri2.to_string () == "http://www.appinf.com/search.cgi?keyword=test%20encoded&scope=all#result");
	}

	TEST_CASE("test other") {
		// The search string is "hello%world"; google happens to ignore the '%'
		// character, so it finds lots of hits for "hello world" programs. This is
		// a convenient reproduction case, and a URL that actually works.
		uri uri1 ("http://google.com/search?q=hello%25world#frag%20ment");

		REQUIRE (uri1.get_host () == "google.com");
		REQUIRE (uri1.get_path () == "/search");
		REQUIRE (uri1.get_query () == "q=hello%world");
		REQUIRE (uri1.get_raw_query () == "q=hello%25world");
		REQUIRE (uri1.get_fragment () == "frag ment");
		REQUIRE (uri1.to_string () == "http://google.com/search?q=hello%25world#frag%20ment");
		REQUIRE (uri1.get_path_etc () == "/search?q=hello%25world#frag%20ment");

		uri1.set_query ("q=foo&bar");
		REQUIRE (uri1.get_query () == "q=foo&bar");
		REQUIRE (uri1.get_raw_query () == "q=foo&bar");
		REQUIRE (uri1.to_string () == "http://google.com/search?q=foo&bar#frag%20ment");
		REQUIRE (uri1.get_path_etc () == "/search?q=foo&bar#frag%20ment");

		uri1.set_query ("q=foo/bar");
		REQUIRE (uri1.get_query () == "q=foo/bar");
		REQUIRE (uri1.get_raw_query () == "q=foo%2Fbar");
		REQUIRE (uri1.to_string () == "http://google.com/search?q=foo%2Fbar#frag%20ment");
		REQUIRE (uri1.get_path_etc () == "/search?q=foo%2Fbar#frag%20ment");

		uri1.set_query ("q=goodbye cruel world");
		REQUIRE (uri1.get_query () == "q=goodbye cruel world");
		REQUIRE (uri1.get_raw_query () == "q=goodbye%20cruel%20world");
		REQUIRE (uri1.to_string () == "http://google.com/search?q=goodbye%20cruel%20world#frag%20ment");
		REQUIRE (uri1.get_path_etc () == "/search?q=goodbye%20cruel%20world#frag%20ment");

		uri1.set_raw_query ("q=pony%7eride");
		REQUIRE (uri1.get_query () == "q=pony~ride");
		REQUIRE (uri1.get_raw_query () == "q=pony%7eride");
		REQUIRE (uri1.to_string () == "http://google.com/search?q=pony%7eride#frag%20ment");
		REQUIRE (uri1.get_path_etc () == "/search?q=pony%7eride#frag%20ment");

		uri1.add_query_parameter ("pa=ra&m1");
		REQUIRE (uri1.get_raw_query () == "q=pony%7eride&pa%3Dra%26m1=");
		REQUIRE (uri1.get_query () == "q=pony~ride&pa=ra&m1=");
		uri1.add_query_parameter ("pa=ra&m2", "val&ue");
		REQUIRE (uri1.get_raw_query () == "q=pony%7eride&pa%3Dra%26m1=&pa%3Dra%26m2=val%26ue");
		REQUIRE (uri1.get_query () == "q=pony~ride&pa=ra&m1=&pa=ra&m2=val&ue");

		uri1 = "http://google.com/search?q=hello+world#frag%20ment";
		REQUIRE (uri1.get_host () == "google.com");
		REQUIRE (uri1.get_path () == "/search");
		REQUIRE (uri1.get_query () == "q=hello+world");
		REQUIRE (uri1.get_raw_query () == "q=hello+world");
		REQUIRE (uri1.get_fragment () == "frag ment");
		REQUIRE (uri1.to_string () == "http://google.com/search?q=hello+world#frag%20ment");
		REQUIRE (uri1.get_path_etc () == "/search?q=hello+world#frag%20ment");
	}

	TEST_CASE("test encode/decode") {
		std::string str;
		uri::encode ("http://google.com/search?q=hello+world#frag ment", "+#?", str);
		REQUIRE (str == "http://google.com/search%3Fq=hello%2Bworld%23frag%20ment");

		str = "";
		uri::encode ("http://google.com/search?q=hello+world#frag ment", "", str);
		REQUIRE (str == "http://google.com/search?q=hello+world#frag%20ment");

		str = "";
		uri::decode ("http://google.com/search?q=hello+world#frag%20ment", str, true);
		REQUIRE (str == "http://google.com/search?q=hello world#frag ment");

		str = "";
		uri::decode ("http://google.com/search?q=hello%2Bworld#frag%20ment", str);
		REQUIRE (str == "http://google.com/search?q=hello+world#frag ment");
	}

	TEST_CASE("test from path") {
		path path1 ("/var/www/site/index.html", path::PATH_UNIX);
		uri uri1 (path1);
		REQUIRE (uri1.to_string () == "file:///var/www/site/index.html");

		path path2 ("/var/www/site/with space.html", path::PATH_UNIX);
		uri uri2 (path2);
		REQUIRE (uri2.to_string () == "file:///var/www/site/with%20space.html");

		path path3 ("c:\\www\\index.html", path::PATH_WINDOWS);
		uri uri3 (path3);
		REQUIRE (uri3.to_string () == "file:///c:/www/index.html");
	}

	TEST_CASE("test query params") {
		uri uri1 ("http://google.com/search?q=hello+world&client=safari");
		uri::query_params_t params = uri1.get_query_parameters ();
		REQUIRE (params.size () == 2);
		REQUIRE (params[0].first == "q");
		REQUIRE (params[0].second == "hello world");
		REQUIRE (params[1].first == "client");
		REQUIRE (params[1].second == "safari");

		uri1.set_query_parameters (params);
		REQUIRE (uri1.to_string () == "http://google.com/search?q=hello%20world&client=safari");

		uri1 = "http://google.com/search?q=&client&";
		params = uri1.get_query_parameters ();
		REQUIRE (params.size () == 2);
		REQUIRE (params[0].first == "q");
		REQUIRE (params[0].second == "");
		REQUIRE (params[1].first == "client");
		REQUIRE (params[1].second == "");

		uri1.set_query_parameters (params);
		REQUIRE (uri1.to_string () == "http://google.com/search?q=&client=");

		params[0].second = "foo/bar?";
		uri1.set_query_parameters (params);
		REQUIRE (uri1.to_string () == "http://google.com/search?q=foo%2Fbar%3F&client=");

		params[0].second = "foo&bar";
		uri1.set_query_parameters (params);
		REQUIRE (uri1.to_string () == "http://google.com/search?q=foo%26bar&client=");
	}
}