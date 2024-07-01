add_subdirectory(doctest_runner)

add_executable(unittest
        main.cc
        mz/explode_test.cc
)

target_link_libraries(unittest neutrino::assets doctest_runner)
target_include_directories(unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
