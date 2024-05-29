#include <iostream>
#include "xml/epp/xml_handler.hh"

namespace bsw
{
	namespace xml
	{
		std::ostream& operator << (std::ostream& os, standalone_t s)
		{
			switch (s)
			{
			case standalone_t::no:
				os << "no";
				break;
			case standalone_t::yes:
				os << "yes";
				break;
			case standalone_t::unknown:
				os << "unknown";
				break;
			}
			return os;
		}
		// ================================================================
		attributes::attributes(const char** atts)
		{
			for (int i = 0; *atts; ++i) 
			{
				const auto k = *atts++;
				const auto v = *atts++;
				m_map.insert(map_t::value_type(k, v));
			}
		}
		// ----------------------------------------------------------------
		bool attributes::exists(const std::string& key) const
		{
			return m_map.find(key) != m_map.end();
		}
		// ----------------------------------------------------------------
		std::string attributes::get(const std::string& key) const
		{
			auto i = m_map.find(key);
			if (i != m_map.end())
			{
				return i->second;
			}
			return "";
		}
		// ----------------------------------------------------------------
		std::string attributes::operator [] (const std::string& key) const
		{
			return get(key);
		}
	} // ns xml
} // ns bsw
