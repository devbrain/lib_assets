if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
        NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if (DEFINED assets_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${assets_SHARED_LIBS}")
endif ()

add_library(assets
        ${PROJECT_INCLUDE}/assets/assets_export.h
        ${PROJECT_INCLUDE}/assets/init.hh
        init.cc
        ${PROJECT_INCLUDE}/assets/assets.hh
        ${PROJECT_INCLUDE}/assets/resources/detail/istream_pos_keeper.hh
        ${PROJECT_INCLUDE}/assets/resources/abstract_resource_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/data_loader.hh
        # --------------------------------------------------------
        # EXPLODE
        # --------------------------------------------------------
        ${PROJECT_INCLUDE}/assets/resources/exe/exe_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/unpacked_exe.hh

        mz/exe_data_loader.cc
        mz/explode/exe_file.cc
        mz/explode/exe_file.hh
        mz/explode/io.cc
        mz/explode/io.hh
        mz/explode/knowledge_dynamics.cc
        mz/explode/knowledge_dynamics.hh
        mz/explode/struct_reader.hh
        mz/explode/unexepack.cc
        mz/explode/unexepack.hh
        mz/explode/unlzexe.cc
        mz/explode/unlzexe.hh
        mz/explode/unpklite.cc
        mz/explode/unpklite.hh


        # --------------------------------------------------------
        # WINRES
        # --------------------------------------------------------
        ${PROJECT_INCLUDE}/assets/resources/exe/windows_resources.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/resource_directory.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_dialog.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_icon_group.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_manifest.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_message_table.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_string_table.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_version.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres_data_loader.hh

        mz/win_exe/abstract_reporter.cc
        mz/win_exe/abstract_reporter.hh
        mz/win_exe/istream_wrapper.cc
        mz/win_exe/istream_wrapper.hh
        mz/win_exe/ms_exe_file_type.cc
        mz/win_exe/ms_exe_file_type.hh
        mz/win_exe/ms_file.hh
        mz/win_exe/pe_resource_builder.cc
        mz/win_exe/pe_resource_builder.hh
        mz/win_exe/resource_directory.cc
        mz/win_exe/windows_ne_file.cc
        mz/win_exe/windows_ne_file.hh
        mz/win_exe/windows_pe_file.cc
        mz/win_exe/windows_pe_file.hh
        mz/win_exe/windows_pe_structs.cc
        mz/win_exe/windows_pe_structs.hh
        mz/win_exe/winres/rs_dialog.cc
        mz/win_exe/winres/rs_icon_group.cc
        mz/win_exe/winres/rs_manifest.cc
        mz/win_exe/winres/rs_message_table.cc
        mz/win_exe/winres/rs_string_table.cc
        mz/win_exe/winres/rs_version.cc
        mz/windows_resources_loader.cc
        mz/windows_resources_loader.hh
        mz/winres_data_loader.cc

        # ---------------------------------------------------------
        # Image
        # ---------------------------------------------------------
        ${PROJECT_INCLUDE}/assets/resources/image/image_data_loader.hh

        image/thirdparty/stb_image.h
        image/stb_wrapper.cc
        image/stb_wrapper.hh
        image/standard_image_resource_loader.hh
        image/image_data_loader.cc

        # -------------------------------------------------------------
        # AUDIO
        # -------------------------------------------------------------

        ${PROJECT_INCLUDE}/assets/resources/audio/music_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/audio/sound_effect_data_loader.hh
        audio/music_data_loader.cc
        audio/sound_effect_data_loader.cc

        # -------------------------------------------------------------
        # FONTS
        # -------------------------------------------------------------

        ${PROJECT_INCLUDE}/assets/resources/font/bgi_font.hh
        ${PROJECT_INCLUDE}/assets/resources/font/bgi_font_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/font/ttf_data_loader.hh

        font/bgi_font_data_loader.cc
        font/ttf_data_loader.cc

        # -------------------------------------------------------------
        # TILESET
        # -------------------------------------------------------------

        ${PROJECT_INCLUDE}/assets/resources/tileset/tileset.hh
        ${PROJECT_INCLUDE}/assets/resources/tileset/tileset_data_loader.hh

        tileset/tileset.cc
        tileset/tileset_data_loader.cc
        tileset/win_icon_loader.cc
        tileset/win_icon_loader.hh

        # ------------------------------------------------------------------
        # WORLD
        # ------------------------------------------------------------------

        world/layers_builder.cc
        world/texture_atlas.cc
        world/texture_atlas_builder.cc
        world/tmx.cc
        world/tmx/animation.cc
        world/tmx/animation.hh
        world/tmx/cell.cc
        world/tmx/cell.hh
        world/tmx/color.hh
        world/tmx/component.cc
        world/tmx/component.hh
        world/tmx/csv/parser.hpp
        world/tmx/group.cc
        world/tmx/group.hh
        world/tmx/image.cc
        world/tmx/image.hh
        world/tmx/image_layer.cc
        world/tmx/image_layer.hh
        world/tmx/json_reader.cc
        world/tmx/json_reader.hh
        world/tmx/layer.cc
        world/tmx/layer.hh
        world/tmx/map.cc
        world/tmx/map.hh
        world/tmx/object.cc
        world/tmx/object.hh
        world/tmx/object_layer.cc
        world/tmx/object_layer.hh
        world/tmx/parse_data.cc
        world/tmx/parse_data.hh
        world/tmx/reader.cc
        world/tmx/reader.hh
        world/tmx/terrain.cc
        world/tmx/terrain.hh
        world/tmx/tile.cc
        world/tmx/tile.hh
        world/tmx/tile_layer.cc
        world/tmx/tile_layer.hh
        world/tmx/tile_set.cc
        world/tmx/tile_set.hh
        world/tmx/wang_set.cc
        world/tmx/wang_set.hh
        world/tmx/xml_reader.cc
        world/tmx/xml_reader.hh
        world/world.cc
        world/world_builder.cc
        world/world_layer.cc
)

add_library(neutrino::assets ALIAS assets)

set_target_properties(assets PROPERTIES
        VERSION ${assets_VERSION}
        SOVERSION ${assets_VERSION_MAJOR})

target_include_directories(assets
        PUBLIC ${PROJECT_INCLUDE} ${PROJECT_CONFIG_INCLUDE}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(assets PUBLIC cxx_std_17)

include(GenerateExportHeader)
generate_export_header(assets EXPORT_FILE_NAME ${PROJECT_CONFIG_INCLUDE}/assets/assets_export_def.h)
target_compile_definitions(assets PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:assets_STATIC_DEFINE>")
target_link_libraries(assets
        PUBLIC neutrino::sdlpp thirdparty::sdl2_ttf thirdparty::sdl2_image thirdparty::sdl2_mixer
        PRIVATE thirdparty::json thirdparty::pugixml
)