add_executable(unittest
        main.cc
        mz/explode_test.cc
        image/image_test.cc
)

target_link_libraries(unittest neutrino::assets thirdparty::doctest)
target_include_directories(unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
