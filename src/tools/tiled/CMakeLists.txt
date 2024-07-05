add_executable(tiled
        main.cc
        world_renderer/types.hh
        world_renderer/view_port.hh
        world_renderer/world_renderer.cc
        world_renderer/world_renderer.hh
        world_renderer/texture_atlas.cc
        world_renderer/texture_atlas.hh
        world_renderer/image_layer.cc
        world_renderer/image_layer.hh
        world_renderer/tile.hh
        world_renderer/tile.cc
        world_renderer/tiles_layer.cc
        world_renderer/tiles_layer.hh
        world_renderer/color_layer.cc
        world_renderer/color_layer.hh
        world_renderer/world_model.cc
        world_renderer/world_model.hh
        world_renderer/world_loader.cc
        world_renderer/world_loader.hh
        world_renderer/animation_sequence.cc
        world_renderer/animation_sequence.hh
        world_renderer/atlas_builder.hh
)
target_link_libraries(tiled PRIVATE assets ${TEST_IMPL_LIB} thirdparty::sdl2-main)
target_include_directories(tiled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

