add_executable(tileset_loader tileset_drv.cc)
target_link_libraries(tileset_loader PRIVATE assets ${TEST_IMPL_LIB})
