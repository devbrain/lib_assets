if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
        NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if (DEFINED assets_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${assets_SHARED_LIBS}")
endif ()

add_library(assets
        datastore/path.cc
        ${PROJECT_INCLUDE}/assets/datastore/path.hh
        datastore/uri.cc
        ${PROJECT_INCLUDE}/assets/datastore/uri.hh
        datastore/file.cc
        ${PROJECT_INCLUDE}/assets/datastore/file.hh
        datastore/file_unix.cc
        ${PROJECT_INCLUDE}/assets/datastore/file_unix.hh
        datastore/directory_iterator_unix.cc
        ${PROJECT_INCLUDE}/assets/datastore/directory_iterator_unix.hh
        datastore/directory_iterator.cc
        ${PROJECT_INCLUDE}/assets/datastore/directory_iterator.hh
        datastore/file_operations.cc
        ${PROJECT_INCLUDE}/assets/datastore/file_operations.hh
        datastore/filesystem.cc
        ${PROJECT_INCLUDE}/assets/datastore/filesystem.hh
        datastore/directory_iterator_operations.cc
        ${PROJECT_INCLUDE}/assets/datastore/directory_iterator_operations.hh
        datastore/physical_fs/physical_file_ops.cc
        datastore/physical_fs/physical_file_ops.hh
        datastore/physical_fs/unix_file_ops.hh
        datastore/physical_fs/win32_file_ops.hh
        datastore/physical_fs/physical_dir_iterator_ops.cc
        datastore/physical_fs/physical_dir_iterator_ops.hh
        datastore/physical_fs/physical_fs.cc
        datastore/physical_fs/physical_fs.hh
    )

add_library(neutrino::assets ALIAS assets)

set_target_properties(assets PROPERTIES
        VERSION ${assets_VERSION}
        SOVERSION ${assets_VERSION_MAJOR})

target_include_directories(assets PUBLIC ${PROJECT_INCLUDE} ${PROJECT_CONFIG_INCLUDE})
target_compile_features(assets PUBLIC cxx_std_17)

include(GenerateExportHeader)
generate_export_header(assets EXPORT_FILE_NAME ${PROJECT_CONFIG_INCLUDE}/assets/assets_export.h)
target_compile_definitions(assets PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:assets_STATIC_DEFINE>")
target_link_libraries(assets PUBLIC neutrino::sdlpp)