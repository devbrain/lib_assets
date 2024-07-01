#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_DIALOG_HPP_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_PEFILE_RS_DIALOG_HPP_

#include <map>
#include <vector>
#include <assets/resources/exe/winres/resource_directory.hh>
#include <assets//assets_export.h>

namespace neutrino::assets {
	d_ASSETS_WINDOWS_RESOURCE_TRAITS(windows_rs_dialog, DIALOG, false);

	class ASSETS_EXPORT windows_rs_dialog {
		d_ASSETS_ADD_WINDOWS_RESOURCE_LOADER(windows_rs_dialog);
		public:
			struct font {
				font()
					: m_size(0),
					  m_weight(0),
					  m_italic(false) {
				}

				uint16_t m_size;
				uint16_t m_weight;
				bool m_italic;
				std::wstring m_name;
			};

			struct control {
				uint32_t m_helpid{};
				uint32_t m_extstyle{};
				uint32_t m_style{};
				uint16_t m_x{};
				uint16_t m_y{};
				uint16_t m_w{};
				uint16_t m_h{};
				uint32_t m_id{};
				windows_resource_name m_ctl_class;
				windows_resource_name m_title;
				std::vector <char> m_extra;
				std::vector<windows_resource_name> m_extra_resources;
			};

		public:
			windows_rs_dialog()
				: m_extended(false),
				  m_helpid(0),
				  m_extstyle(0),
				  m_style(0),
				  m_x(0),
				  m_y(0),
				  m_w(0),
				  m_h(0) {
			}

		public:
			bool m_extended;
			uint32_t m_helpid;
			uint32_t m_extstyle;
			uint32_t m_style;
			uint16_t m_x;
			uint16_t m_y;
			uint16_t m_w;
			uint16_t m_h;

			windows_resource_name m_menu;
			windows_resource_name m_dlg_class;
			windows_resource_name m_title;

			font m_font;

			std::vector <control> m_control;
	};
}

#endif
