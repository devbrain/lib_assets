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

        mz/explode/io.cc
        mz/explode/io.hh
        mz/explode/exe_file.cc
        mz/explode/exe_file.hh
        mz/explode/knowledge_dynamics.cc
        mz/explode/knowledge_dynamics.hh
        mz/explode/struct_reader.hh
        mz/explode/unexepack.cc
        mz/explode/unexepack.hh
        mz/explode/unlzexe.cc
        mz/explode/unlzexe.hh
        mz/explode/unpklite.cc
        mz/explode/unpklite.hh


        mz/win_exe/winres/rs_icon_group.cc
        mz/win_exe/winres/rs_dialog.cc
        mz/win_exe/pe_resource_builder.hh
        mz/win_exe/windows_ne_file.cc
        mz/win_exe/windows_ne_file.hh
        mz/win_exe/ms_file.hh
        mz/winres_data_loader.cc
        mz/windows_resources_loader.cc
        mz/windows_resources_loader.hh

        ${PROJECT_INCLUDE}/assets/resources/exe/winres/resource_directory.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_manifest.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_icon_group.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_message_table.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_version.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_string_table.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres/rs_dialog.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/windows_resources.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/winres_data_loader.hh


        mz/win_exe/windows_pe_file.cc
        mz/win_exe/windows_pe_file.hh
        mz/win_exe/windows_pe_structs.cc
        mz/win_exe/windows_pe_structs.hh

        mz/win_exe/ms_exe_file_type.cc
        mz/win_exe/ms_exe_file_type.hh

        mz/win_exe/pe_resource_builder.cc
        mz/win_exe/winres/rs_string_table.cc

        mz/win_exe/resource_directory.cc
        mz/win_exe/abstract_reporter.hh


        mz/win_exe/abstract_reporter.cc


        mz/win_exe/winres/rs_message_table.cc
        mz/win_exe/winres/rs_manifest.cc
        mz/win_exe/winres/rs_version.cc

        mz/win_exe/istream_wrapper.cc
        mz/win_exe/istream_wrapper.hh


        image/thirdparty/stb_image.h
        image/stb_wrapper.cc
        image/stb_wrapper.hh
        image/standard_image_resource_loader.hh


        ${PROJECT_INCLUDE}/assets/resources/detail/istream_pos_keeper.hh
        ${PROJECT_INCLUDE}/assets/resources/abstract_resource_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/data_loader.hh

        ${PROJECT_INCLUDE}/assets/resources/image/image_data_loader.hh

        ${PROJECT_INCLUDE}/assets/resources/exe/exe_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/unpacked_exe.hh
        image/image_data_loader.cc
        mz/exe_data_loader.cc


        ${PROJECT_INCLUDE}/assets/resources/audio/music_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/audio/sound_effect_data_loader.hh
        audio/music_data_loader.cc
        audio/sound_effect_data_loader.cc

        ${PROJECT_INCLUDE}/assets/resources/font/bgi_font.hh
        ${PROJECT_INCLUDE}/assets/resources/font/bgi_font_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/font/ttf_data_loader.hh
        font/bgi_font_data_loader.cc
        font/ttf_data_loader.cc

        ${PROJECT_INCLUDE}/assets/resources/tileset/tileset.hh
        ${PROJECT_INCLUDE}/assets/resources/tileset/tileset_data_loader.hh
        tileset/tileset.cc
        tileset/tileset_data_loader.cc
        tileset/win_icon_loader.cc
        tileset/win_icon_loader.hh


#        tmx/animation.cc
#        tmx/animation.hh
#        tmx/cell.cc
#        tmx/cell.hh
#        tmx/color.hh
#        tmx/component.cc
#        tmx/component.hh
#        tmx/group.cc
#        tmx/group.hh
#        tmx/image.cc
#        tmx/image.hh
#        tmx/image_layer.cc
#        tmx/image_layer.hh
#        tmx/json_reader.cc
#        tmx/json_reader.hh
#        tmx/layer.cc
#        tmx/layer.hh
#        tmx/map.cc
#        tmx/map.hh
#        tmx/object.cc
#        tmx/object.hh
#        tmx/object_layer.cc
#        tmx/object_layer.hh
#        tmx/parse_data.cc
#        tmx/parse_data.hh
#        tmx/reader.cc
#        tmx/reader.hh
#        tmx/terrain.cc
#        tmx/terrain.hh
#        tmx/tile.cc
#        tmx/tile.hh
#        tmx/tile_layer.cc
#        tmx/tile_layer.hh
#        tmx/tile_set.cc
#        tmx/tile_set.hh
#        tmx/tmx.cc
#        tmx/wang_set.cc
#        tmx/wang_set.hh
#        tmx/xml_reader.cc
#        tmx/xml_reader.hh
#        tmx/csv/parser.hpp

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