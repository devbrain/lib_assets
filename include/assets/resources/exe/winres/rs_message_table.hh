#ifndef __PEFILE_RS_MESSAGE_TABLE_HPP__
#define __PEFILE_RS_MESSAGE_TABLE_HPP__

#include <map>
#include <string>
#include <assets/resources/exe/winres/resource_directory.hh>


namespace assets::pefile {
	class ASSETS_EXPORT message_table {
	 public:
		static constexpr int resource_id () {
			return 11;
		}

		static constexpr bool singleton () {
			return false;
		}

	 public:
		typedef std::map<uint16_t, std::wstring> msg_table_t;
	 public:
		[[nodiscard]] msg_table_t::const_iterator begin () const;
		[[nodiscard]] msg_table_t::const_iterator end () const;

		[[nodiscard]] bool exists (uint16_t key) const;
		std::wstring operator[] (uint16_t key) const;

		static void load (const windows_pe_file& file, const resource& rn, message_table& out);
	 private:
		std::map<uint16_t, std::wstring> m_messages;
	};

} // ns pefile

#endif