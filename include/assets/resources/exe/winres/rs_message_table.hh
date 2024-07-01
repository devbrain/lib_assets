#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_MESSAGE_TABLE_HPP_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_MESSAGE_TABLE_HPP_

#include <map>
#include <string>
#include <assets/resources/exe/winres/resource_directory.hh>


namespace neutrino::assets {
	d_ASSETS_WINDOWS_RESOURCE_TRAITS(windows_rs_message_table, MESSAGETABLE, false);
	class ASSETS_EXPORT windows_rs_message_table {
		d_ASSETS_ADD_WINDOWS_RESOURCE_LOADER(windows_rs_message_table);
	 public:
		typedef std::map<uint16_t, std::wstring> msg_table_t;
	 public:
		[[nodiscard]] msg_table_t::const_iterator begin () const;
		[[nodiscard]] msg_table_t::const_iterator end () const;

		[[nodiscard]] bool exists (uint16_t key) const;
		std::wstring operator[] (uint16_t key) const;
	 private:
		std::map<uint16_t, std::wstring> m_messages;
	};

} // ns pefile

#endif