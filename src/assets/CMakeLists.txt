function(BIN2C input_file RESULT_NAME)
    get_filename_component(INPUT_DIR ${input_file} DIRECTORY)
    get_filename_component(INPUT_NAME ${input_file} NAME_WE)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_DIR})
    set(ONAME ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_DIR}/${INPUT_NAME}.h)
    add_custom_command(
            OUTPUT ${ONAME}
            COMMAND $<TARGET_FILE:bin2c> ${CMAKE_CURRENT_SOURCE_DIR}/${input_file} ${ONAME} ${INPUT_NAME}
            DEPENDS bin2c
            MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${input_file}
    )
    set(${RESULT_NAME} ${ONAME} PARENT_SCOPE)
endfunction()


if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
        NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if (DEFINED assets_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${assets_SHARED_LIBS}")
endif ()

set(TEST_DATA
        test1/example.tmx
        test1/js_example.json
        test1/test_base64_level.tmx

        test1/test_csv_level.tmx
        test1/test_xml_level.tmx
        test-rs/tiled_flipped.tmx
        test-rs/tilesheet.tsx
        test-rs/tiled_base64_zlib.tmx
        test-rs/tiled_base64_gzip.tmx
        test-rs/js_tiled_base64_gzip.json
        test-rs/tiled_base64_zstandard.tmx
        test3/b64zlib.tmx
        test-rs/tiled_base64_zlib_infinite.tmx
        test-rs/js_tiled_base64_zlib_infinite.json
        test-rs/tilesheet.tsx
        test-rs/tiled_image_layers.tmx
        test-rs/js_tiled_image_layers.json
        test4/example.tmx
        test4/js_example.json
        json/simple_map.json
        wang/isometric_staggered_grass_and_water.tmx
        wang/js_isometric_staggered_grass_and_water.json
)

set(generated_test_headers)
foreach (FILE ${TEST_DATA})
    bin2c("world/tmx/test/data/${FILE}" generated)
    list(APPEND generated_test_headers ${generated})
endforeach ()

set(tmx_test_src
        world/tmx/test/test1_example_tmx.cc
        world/tmx/test/test_color.cc
        world/tmx/test/test_compression.cc
        world/tmx/test/test_encodings.cc
        world/tmx/test/test_flipped.cc
        world/tmx/test/test_image_layers.cc
        world/tmx/test/test_infinite.cc
        world/tmx/test/test_json_complex.cc
        world/tmx/test/test_json_simple.cc
        world/tmx/test/test_properties.cc
        world/tmx/test/test_text.cc
        world/tmx/test/test_utils.cc
        world/tmx/test/test_utils.hh
        world/tmx/test/test_wang.cc
        ${generated_test_headers}
)


set(assets_src
        ${PROJECT_INCLUDE}/assets/assets_export.h
        ${PROJECT_INCLUDE}/assets/init.hh
        init.cc
        resource_from_stream.cc
        ${PROJECT_INCLUDE}/assets/assets.hh
        ${PROJECT_INCLUDE}/assets/resources/detail/istream_pos_keeper.hh
        ${PROJECT_INCLUDE}/assets/resources/detail/bitmap2d.hh
        ${PROJECT_INCLUDE}/assets/resources/detail/bitstream.hh
        ${PROJECT_INCLUDE}/assets/resources/abstract_resource_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/loader_state.hh
        ${PROJECT_INCLUDE}/assets/resources/resource_from_stream.hh
        detail/bitstream.cc
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
        ${PROJECT_INCLUDE}/assets/resources/image/ega_image.hh
        ${PROJECT_INCLUDE}/assets/resources/image/image_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/image/pixels.hh



        image/thirdparty/stb_image.h
        image/stb_wrapper.cc
        image/stb_wrapper.hh
        image/standard_image_resource_loader.hh
        image/image_data_loader.cc
        image/pixels.cc
        image/ega_image.cc
        image/raw_image_loader.cc
        image/raw_image_loader.hh

        # -------------------------------------------------------------
        # PALETTE
        # -------------------------------------------------------------
        ${PROJECT_INCLUDE}/assets/resources/palette/palette_data_loader.hh
        palette/palette_data_loader.cc
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
        ${PROJECT_INCLUDE}/assets/resources/font/rom_font.hh
        ${PROJECT_INCLUDE}/assets/resources/font/rom_font_data_loader.hh

        font/bgi_font_data_loader.cc
        font/ttf_data_loader.cc
        font/rom_font.cc
        font/rom_font_data_loader.cc
        font/bgi/bold_chr.h
        font/bgi/euro_chr.h
        font/bgi/goth_chr.h
        font/bgi/lcom_chr.h
        font/bgi/litt_chr.h
        font/bgi/sans_chr.h
        font/bgi/scri_chr.h
        font/bgi/simp_chr.h
        font/bgi/trip_chr.h
        font/bgi/tschr_chr.h
        # -------------------------------------------------------------
        # TILESET
        # -------------------------------------------------------------

        ${PROJECT_INCLUDE}/assets/resources/tileset/tileset.hh
        ${PROJECT_INCLUDE}/assets/resources/tileset/tileset_data_loader.hh

        tileset/tileset.cc
        tileset/tileset_data_loader.cc
        tileset/win_icon_loader.cc
        tileset/win_icon_loader.hh
        tileset/prographx_loader.cc
        tileset/prographx_loader.hh

        # ------------------------------------------------------------------
        # WORLD
        # ------------------------------------------------------------------
        ${PROJECT_INCLUDE}/assets/resources/world/tile_description.hh
        ${PROJECT_INCLUDE}/assets/resources/world/types.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_animation_sequence.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_data_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_component.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_object.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_object_layer.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_tiles_layer.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_tiles_set.hh
        ${PROJECT_INCLUDE}/assets/resources/world/world_image_layer.hh


        world/path_resolver.hh
        world/tmx_loader.cc
        world/tmx_loader.hh
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
        world/world_animation_sequence.cc
        world/world_data_loader.cc
        world/world_component.cc
        world/world_object_layer.cc
        world/world_tiles_layer.cc
        world/world_tiles_set.cc
        world/world_image_layer.cc
)

if (assets_ENABLE_TESTING)
    set (assets_test_src ${tmx_test_src})
    set (test_libs doctest_runner thirdparty::doctest)
    set (test_include ${CMAKE_CURRENT_BINARY_DIR})
endif()


add_library(assets ${assets_src} ${assets_test_src})

add_library(neutrino::assets ALIAS assets)

set_target_properties(assets PROPERTIES
        VERSION ${assets_VERSION}
        SOVERSION ${assets_VERSION_MAJOR})

target_include_directories(assets
        PUBLIC ${PROJECT_INCLUDE} ${PROJECT_CONFIG_INCLUDE}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${test_include}
)

target_compile_features(assets PUBLIC cxx_std_17)

include(GenerateExportHeader)
generate_export_header(assets EXPORT_FILE_NAME ${PROJECT_CONFIG_INCLUDE}/assets/assets_export_def.h)
target_compile_definitions(assets PRIVATE LOGGER_MODULE_NAME=neutrino::assets)
if (assets_ENABLE_TESTING)
    target_compile_definitions(assets PRIVATE ASSETS_BUILD_FOR_TESTING)
endif()
target_link_libraries(assets
        PUBLIC neutrino::sdlpp thirdparty::sdl2_ttf thirdparty::sdl2_image thirdparty::sdl2_mixer thirdparty::dynamic_bitset ${test_libs}
               thirdparty::json thirdparty::pugixml
)

if (MSVC)
    target_compile_options(assets PRIVATE /W4)
else()
    target_compile_options(assets PRIVATE -Wall -Wextra -pedantic -Werror)
endif()


