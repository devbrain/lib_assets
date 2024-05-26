if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
        NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if (DEFINED assets_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${assets_SHARED_LIBS}")
endif ()

add_library(assets
        ${PROJECT_INCLUDE}/assets/mz/explode_exe_file.hh
        explode/mz/exe_file.cc
        explode/mz/exe_file.hh
        explode/mz/io.cc
        explode/mz/io.hh
        explode/mz/knowledge_dynamics.cc
        explode/mz/knowledge_dynamics.hh
        explode/mz/struct_reader.hh
        explode/mz/unexepack.cc
        explode/mz/unexepack.hh
        explode/mz/unlzexe.cc
        explode/mz/unlzexe.hh
        explode/mz/unpklite.cc
        explode/mz/unpklite.hh
        explode/explode_exe_file.cc
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