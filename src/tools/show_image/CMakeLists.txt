add_executable(show_image main.cc)
target_link_libraries(show_image PRIVATE assets thirdparty::sdl2-main ${TEST_IMPL_LIB})