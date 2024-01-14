add_executable(unittest
        main.cc
        datastore/path_test.cc
        datastore/uri_test.cc
        datastore/file_test.cc
        datastore/directory_iterator_test.cc
)
target_link_libraries(unittest neutrino::assets thirdparty::doctest)
