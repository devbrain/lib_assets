include(FetchContent)

set(FETCHCONTENT_QUIET off)

if (assets_ENABLE_TESTING)
    add_subdirectory(doctest)
endif()

add_subdirectory(sdlpp)
add_subdirectory(dynamic_bitset)



