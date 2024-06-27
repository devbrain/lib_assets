#include <assets/resources/exe/winres/rs_dialog.hh>
#include <bsw/exception.hh>
#include "mz/win_exe/istream_wrapper.hh"
#include "mz/win_exe/ms_file.hh"
#include <bsw/strings/wchar.hh>

namespace neutrino::assets {
	static windows_resource_name load_resource_name_dlg(bsw::istream_wrapper& is, std::size_t maxlen, bool is_pe) {
		if (is_pe) {
			uint16_t descr;
			is >> descr;
			if (descr == 0) {
				return {};
			} else {
				if (descr == 0xFFFF) {
					uint16_t id;
					is >> id;
					return windows_resource_name(id);
				} else {
					auto ch = static_cast <wchar_t>(descr);
					std::wstring nm;
					nm += ch;
					bool term = false;

					for (std::size_t i = 0; i < maxlen; i++) {
						is >> ch;
						if (ch) {
							nm += ch;
						} else {
							term = true;
							break;
						}
					}
					if (!term) {
						RAISE_EX("String should be null terminated");
					}
					return windows_resource_name(nm);
				}
			}
		} else {
			uint8_t flag;
			is >> flag;
			if (flag == 0) {
				return {};
			}
			if (flag == 0xFF) {
				uint16_t id;
				is >> id;
				return windows_resource_name(id);
			} else {
				std::string name;
				bool term = false;
				name += static_cast <char>(flag);
				for (std::size_t i = 1; i < maxlen; i++) {
					char ch;
					is >> ch;
					if (ch) {
						name += ch;
					} else {
						term = true;
						break;
					}
				}
				if (!term) {
					RAISE_EX("String should be null terminated");
				}
				return windows_resource_name(bsw::utf8_to_wstring(name));
			}
		}
	}

	using word = uint16_t;
	using dword = uint32_t;
	using byte = uint8_t;

	struct ne_dialog_control {
		word x;
		word y;
		word width;
		word height;
		word id;
		dword style;
		byte cls;
	};

	// ---------------------------------------------------------------------------------
	void windows_resource_traits <windows_rs_dialog>::load(const ms_file& file, const windows_resource& rn,
	                                                       windows_rs_dialog& out) {
		const std::size_t file_size = file.file_size();
		auto offs = file.offset_in_file(rn.offset());

		if (offs >= file_size) {
			return;
		}

		bsw::istream_wrapper is(file.stream(), offs, rn.size());

		union {
			uint16_t w[2];
			uint32_t dw;
		} u;
		is >> u.dw;
		if (file.is_pe()) {
			if (u.w[0] == 1 && u.w[1] == 0xFFFF) {
				out.m_extended = true;
				is >> out.m_helpid >> out.m_extstyle >> out.m_style;
			} else {
				out.m_style = u.dw;
				is >> out.m_extstyle;
			}
		} else {
			out.m_style = u.dw;
		}
		uint16_t count;
		if (file.is_pe()) {
			is >> count;
		} else {
			uint8_t ne_count;
			is >> ne_count;
			count = ne_count;
		}
		is >> out.m_x >> out.m_y >> out.m_w >> out.m_h;

		out.m_menu = load_resource_name_dlg(is, rn.size(), file.is_pe());
		out.m_dlg_class = load_resource_name_dlg(is, rn.size(), file.is_pe());
		out.m_title = load_resource_name_dlg(is, rn.size(), file.is_pe());
		if (out.m_style & 0x00000040) {
			uint16_t fnt;
			is >> fnt;
			if (fnt != 0x7FFF) {
				out.m_font.m_size = fnt;
				if (out.m_extended) {
					is >> out.m_font.m_weight;
					uint16_t it;
					is >> it;
					if (it) {
						out.m_font.m_italic = true;
					}
				}
				if (fnt) {
					if (file.is_pe()) {
						is.read_string(out.m_font.m_name, rn.size(), true);
					} else {
						std::string font_name;
						is.read_string(font_name, rn.size(), true);
						out.m_font.m_name = bsw::utf8_to_wstring(font_name);
					}
				}
			}
			if (file.is_pe()) {
				is.align4();
			}
		}
		for (uint16_t i = 0; i < count; i++) {
			windows_rs_dialog::control ctl;
			if (file.is_pe()) {
				if (out.m_extended) {
					is >> ctl.m_helpid >> ctl.m_extstyle >> ctl.m_style;
				} else {
					is >> ctl.m_extstyle >> ctl.m_style;
				}
				is >> ctl.m_x >> ctl.m_y >> ctl.m_w >> ctl.m_h;

				if (out.m_extended) {
					uint32_t cid;
					is >> cid;
					ctl.m_id = cid;
				} else {
					uint16_t cid;
					is >> cid;
					ctl.m_id = cid;
				}
			} else {
				ne_dialog_control ne_ctl{};
				is >> ne_ctl;
				ctl.m_x = ne_ctl.x;
				ctl.m_y = ne_ctl.y;
				ctl.m_h = ne_ctl.height;
				ctl.m_w = ne_ctl.width;
				ctl.m_id = ne_ctl.id;
				ctl.m_style = ne_ctl.style;
				if (ne_ctl.cls & 0x80) {
					if (ne_ctl.cls <= 0x85) {
						ctl.m_ctl_class = windows_resource_name(ne_ctl.cls & (~0x80));
					} else {
						ctl.m_ctl_class = windows_resource_name(ne_ctl.cls);
					}
				} else {
					ctl.m_ctl_class = load_resource_name_dlg(is, rn.size(), file.is_pe());
				}
			}
			if (file.is_pe()) {
				ctl.m_ctl_class = load_resource_name_dlg(is, rn.size(), file.is_pe());
			}
			ctl.m_title = load_resource_name_dlg(is, rn.size(), file.is_pe());
			if (file.is_pe()) {
				if (is.size_to_end() == 0 && i + 1 == count) {
					is.align4();
					return;
				}
				uint16_t extra_size;
				is >> extra_size;
				if (extra_size) {
					if (extra_size < 2) {
						RAISE_EX("Illegal extrasize");
					}
					ctl.m_extra.resize(extra_size - 2);
					is.read(ctl.m_extra.data(), ctl.m_extra.size());
				}
				is.align4();
			} else {
				auto ern = load_resource_name_dlg(is, rn.size(), file.is_pe());
				if (ern.id() || !ern.name().empty()) {
					ctl.m_extra_resources.push_back(ern);
				}
			}
			out.m_control.push_back(ctl);
		}
	}
}
