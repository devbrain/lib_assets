add_library(doctest_runner SHARED doctest_runner.cc)
target_link_libraries(doctest_runner PUBLIC thirdparty::doctest)
# set_property(TARGET doctest_runner PROPERTY POSITION_INDEPENDENT_CODE ON)