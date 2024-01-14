set(BUILD_SHARED_LIBS TRUE CACHE INTERNAL "")

fetchcontent_declare(
        SDLPP_EXT
        GIT_REPOSITORY https://github.com/devbrain/lib_sdlpp.git
        GIT_PROGRESS TRUE
        GIT_TAG master
        TIMEOUT 10
        UPDATE_COMMAND ""
)

fetchcontent_makeavailable(SDLPP_EXT)

