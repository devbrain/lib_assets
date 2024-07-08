//
// Created by igor on 6/24/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_TTF_FONT_DATA_MANAGER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_TTF_FONT_DATA_MANAGER_HH

#include <assets/resources/data_loader.hh>
#include <assets/resources/resource_from_stream.hh>
#include <sdlpp/video/ttf.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	struct ASSETS_EXPORT ttf_resource : resource_from_stream {
		ttf_resource(std::istream& is, int ptsize_)
			: ptsize(ptsize_),
			  hdpi(0),
			  vdpi(0),
			  m_stream(is) {
		}

		ttf_resource(std::istream& is, int ptsize_, unsigned int hdpi_)
			: ptsize(ptsize_),
			  hdpi(hdpi_),
			  vdpi(0),
			  m_stream(is) {
		}

		ttf_resource(std::istream& is, int ptsize_, unsigned int hdpi_, unsigned int vdpi_)
			: ptsize(ptsize_),
			  hdpi(hdpi_),
			  vdpi(vdpi_),
			  m_stream(is) {
		}

		[[nodiscard]] std::istream& get_stream() const override;

		int ptsize;
		unsigned int hdpi;
		unsigned int vdpi;
		std::istream& m_stream;
	};

	ASSETS_ENABLE_ISTREAM_STATE(ttf_resource);

	class ASSETS_EXPORT ttf_data_loader : public data_loader <sdl::ttf, ttf_resource> {
		public:
			ttf_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_FONT_TTF_FONT_DATA_MANAGER_HH
