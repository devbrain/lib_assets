#ifndef ARES_BSW_XML_HANDLER_HH
#define ARES_BSW_XML_HANDLER_HH

#include <string>
#include <map>
#include <iosfwd>

namespace bsw
{
	namespace xml
	{
		enum class standalone_t
		{
			yes,
			no,
			unknown
		};
		std::ostream& operator << (std::ostream& os, standalone_t s);

		class attributes
		{
		public:
			explicit attributes(const char** atts);

			bool exists(const std::string& key) const;
			std::string get(const std::string& key) const;
			std::string operator [] (const std::string& key) const;
		private:
			using map_t = std::map <std::string, std::string>;
			map_t m_map;
		};
	} // ns xml
} // ns bsw

#endif