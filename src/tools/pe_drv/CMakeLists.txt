add_executable(pe_drv pe_drv.cc)
target_link_libraries(pe_drv PRIVATE assets)
target_include_directories(pe_drv PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../assets)