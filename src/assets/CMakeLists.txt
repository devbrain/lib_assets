if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
        NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if (DEFINED assets_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${assets_SHARED_LIBS}")
endif ()

add_library(assets

        ${PROJECT_INCLUDE}/assets/assets.hh

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

        mz/pefile/clr_res.hh
        mz/pefile/rs_icon_group.cc
        mz/pefile/rs_dialog.cc
        mz/pefile/resource_builder.hh
        mz/pefile/clr.cc
        mz/pefile/resource_directory.hh
        mz/pefile/pefile.hh
        mz/pefile/pefile.cc
        mz/pefile/exefile.hh
        mz/pefile/clr_tables_helper.hh
        mz/pefile/rs_manifest.hh
        mz/pefile/rs_icon_group.hh
        mz/pefile/resource_builder.cc
        mz/pefile/rs_string_table.cc
        mz/pefile/imports_parser.hh
        mz/pefile/clr_tables.hh
        mz/pefile/exefile.cc
        mz/pefile/imports_parser.cc
        mz/pefile/resource_directory.cc
        mz/pefile/abstract_reporter.hh
        mz/pefile/rs_message_table.hh
        mz/pefile/clr_res.cc
        mz/pefile/abstract_reporter.cc
        mz/pefile/rs_version.hh
        mz/pefile/rs_dialog.hh
        mz/pefile/clr.hh
        mz/pefile/rs_message_table.cc
        mz/pefile/rs_manifest.cc
        mz/pefile/rs_version.cc
        mz/pefile/rs_string_table.hh
        mz/pefile/istream_wrapper.cc
        mz/pefile/istream_wrapper.hh

        xml/expat/xmltok.c
        xml/expat/iasciitab.h
        xml/expat/gccconfig.h
        xml/expat/xmltok_impl.c
        xml/expat/macconfig.h
        xml/expat/xmltok_impl.h
        xml/expat/xmlrole.c
        xml/expat/xmltok.h
        xml/expat/expat_external.h
        xml/expat/latin1tab.h
        xml/expat/xmlrole.h
        xml/expat/asciitab.h
        xml/expat/expat.h
        xml/expat/xmlparse.c
        xml/expat/xmltok_ns.c
        xml/expat/utf8tab.h
        xml/expat/ascii.h
        xml/expat/internal.h
        xml/expat/amigaconfig.h
        xml/expat/nametab.h
        xml/expat/winconfig.h
        xml/epp/xml.hh
        xml/epp/xml_parser_base.hh
        xml/epp/xml_handler.cc
        xml/epp/xml_parser_base.cc
        xml/epp/expat_inc.hh
        xml/epp/xml_handler.hh
        xml/epp/xml_handler_iface.hh


        image/bmp.hpp
        image/icons/icons_reader.hpp
        image/icons/ne_image.hpp
        image/icons/pe_image.hpp
        image/icons/ico_image.hpp
        image/icons/ico_image.cpp
        image/icons/pe_image.cpp
        image/icons/extract_icon.hpp
        image/icons/extract_icon.cpp
        image/icons/load_icon.hpp
        image/icons/load_icon.cpp
        image/icons/ne_image.cpp
        image/icons/icons_reader.cpp
        image/icons/icons_container.cpp
        image/icons/icons_container.hpp
        image/bmp.cpp
        image/thirdparty/lodepng.cpp
        image/thirdparty/lodepng.h

        image/thirdparty/stb_image.h
        image/stb_wrapper.cc
        image/stb_wrapper.hh
        image/standard_image_resource_loader.hh
        image/pcx.cc
        image/pcx.hh

        ${PROJECT_INCLUDE}/assets/resources/detail/istream_pos_keeper.hh
        ${PROJECT_INCLUDE}/assets/resources/abstract_resource_loader.hh
        ${PROJECT_INCLUDE}/assets/resources/data_manager.hh

        ${PROJECT_INCLUDE}/assets/resources/image/image_info.hh
        ${PROJECT_INCLUDE}/assets/resources/image/image_data_manager.hh

        ${PROJECT_INCLUDE}/assets/resources/exe/exe_info.hh
        ${PROJECT_INCLUDE}/assets/resources/exe/exe_data_manager.hh
        image/image_data_manager.cc
        mz/exe_data_manager.cc
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
generate_export_header(assets EXPORT_FILE_NAME ${PROJECT_CONFIG_INCLUDE}/assets/assets_export.h)
target_compile_definitions(assets PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:assets_STATIC_DEFINE>")
target_link_libraries(assets PUBLIC neutrino::sdlpp)