add_executable(tiled main.cc)
target_link_libraries(tiled PRIVATE assets ${TEST_IMPL_LIB} thirdparty::sdl2-main)

