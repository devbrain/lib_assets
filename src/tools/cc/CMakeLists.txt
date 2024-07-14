add_executable(cc
        main.cc

        raw_map.cc
        raw_map.hh
        map_tile.hh
        dm.cc
        dm.hh
        draw_map.cc
        draw_map.hh

        crystal_caves/cc_decode.cc
        crystal_caves/cc_decode.hh
        crystal_caves/cc_tiles_mapping.hh
        crystal_caves/crystal_caves.cc
        crystal_caves/crystal_caves.hh

#        secret_agent/sa_key.cc
#        secret_agent/sa_key.hh
#        secret_agent/secret_agent.cc
#        secret_agent/secret_agent.hh
#        secret_agent/sa_tiles_mapping.hh
#        secret_agent/sa_decode.cc
#        secret_agent/sa_decode.hh
        )

target_link_libraries(cc PRIVATE assets ${TEST_IMPL_LIB} thirdparty::sdl2-main)
target_include_directories(cc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
