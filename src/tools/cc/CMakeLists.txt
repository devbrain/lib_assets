add_executable(cc main.cc cc_tiles_mapping.hh)
target_link_libraries(cc  PRIVATE assets ${TEST_IMPL_LIB} thirdparty::sdl2-main)
