//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_

#include <sdlpp/video/surface.hh>
#include <assets/resources/data_loader.hh>
#include <assets/resources/resource_from_stream.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT image_resource : public resource_from_stream {
		public:
			image_resource(std::istream& is);
			[[nodiscard]] std::istream& get_stream() const override;

		private:
			std::istream& m_stream;
	};

	ASSETS_ENABLE_ISTREAM_STATE(image_resource);

	class ASSETS_EXPORT raw_image_resource : public image_resource {
		public:
			enum type {
				VGA,
				EGA
			};

			enum palette {
				NO_PALETTE,
				PALLETE_AT_START,
				PALETTE_AT_END
			};

			raw_image_resource(std::istream& is, type image_type, palette where);
			[[nodiscard]] type get_image_type() const;
			[[nodiscard]] palette where_palette() const;

		private:
			type m_type;
			palette m_where;
	};

	ASSETS_ENABLE_ISTREAM_STATE(raw_image_resource);

	class ASSETS_EXPORT image_data_loader : public data_loader <sdl::surface, image_resource> {
		public:
			image_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_IMAGE_DATA_MANAGER_HH_
