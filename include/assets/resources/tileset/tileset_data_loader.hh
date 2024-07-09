//
// Created by igor on 6/23/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_DATA_LOADER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_DATA_LOADER_HH

#include <assets/resources/tileset/tileset.hh>
#include <assets/resources/data_loader.hh>
#include <assets/resources/resource_from_stream.hh>
#include <assets/assets_export.h>

namespace neutrino::assets {
	class ASSETS_EXPORT tileset_resource : public resource_from_stream {
		public:
			tileset_resource(std::istream& is);
			[[nodiscard]] std::istream& get_stream() const override;

		private:
			std::istream& m_stream;
	};

	ASSETS_ENABLE_ISTREAM_STATE(tileset_resource);

	class ASSETS_EXPORT prographx_resource : public tileset_resource {
		public:
			prographx_resource(std::istream& is, bool masked, std::size_t padding);
			prographx_resource(std::istream& is, bool masked, std::size_t padding, std::size_t max_tiles);

			[[nodiscard]] bool is_masked() const;
			[[nodiscard]] std::size_t get_padding() const;
			[[nodiscard]] std::size_t get_max_tiles() const;

		private:
			bool m_masked;
			std::size_t m_padding;
			std::size_t m_max_tiles;
	};

	ASSETS_ENABLE_ISTREAM_STATE(prographx_resource);

	class ASSETS_EXPORT tileset_data_loader : public data_loader <tileset, tileset_resource> {
		public:
			tileset_data_loader();
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_TILESET_DATA_LOADER_HH
