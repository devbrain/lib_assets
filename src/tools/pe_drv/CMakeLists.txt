add_executable(pe_drv pe_drv.cc)
target_link_libraries(pe_drv PRIVATE assets ${TEST_IMPL_LIB})
