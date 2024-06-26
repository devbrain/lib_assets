#ifndef __PEFILE_RS_ICON_GROUP_HPP__
#define __PEFILE_RS_ICON_GROUP_HPP__

#include <map>
#include <assets/resources/exe/winres/resource_directory.hh>

namespace neutrino::assets {
	d_ASSETS_WINDOWS_RESOURCE_TRAITS(windows_rs_icon_group, GROUP_ICON, false);

	class ASSETS_EXPORT windows_rs_icon_group {
		d_ASSETS_ADD_WINDOWS_RESOURCE_LOADER(windows_rs_icon_group);

		public:
			typedef std::multimap <uint32_t, uint16_t> icon_group_t;

			[[nodiscard]] icon_group_t::const_iterator begin() const;
			[[nodiscard]] icon_group_t::const_iterator end() const;

		private:
			void _add_group(uint32_t group_id, uint16_t ordinal);

		private:
			icon_group_t m_grp;
	};

	d_ASSETS_WINDOWS_RESOURCE_TRAITS(windows_rs_icon_info, ICON, false);

	class ASSETS_EXPORT windows_rs_icon_info {
		d_ASSETS_ADD_WINDOWS_RESOURCE_LOADER(windows_rs_icon_info);

		public:
			windows_rs_icon_info();
			uint16_t ordinal;
			uint64_t offset;
			uint32_t size;

		private:
			windows_rs_icon_info(uint16_t id, uint64_t offs, uint32_t s);
	};
} // ns pefile

#endif
