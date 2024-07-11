add_executable(cc
        main.cc
        raw_map.cc
        raw_map.hh
        cc_decode.cc
        cc_decode.hh
        map_tile.hh
        cc_tiles_mapping.hh)
target_link_libraries(cc PRIVATE assets ${TEST_IMPL_LIB} thirdparty::sdl2-main)
