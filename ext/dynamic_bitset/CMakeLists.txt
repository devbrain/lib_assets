set(DYNAMICBITSET_BUILD_EXAMPLE OFF CACHE INTERNAL "")
set(DYNAMICBITSET_BUILD_TESTS OFF CACHE INTERNAL "")
fetchcontent_declare(
        DBITSET
        GIT_REPOSITORY "https://github.com/pinam45/dynamic_bitset.git"
        GIT_TAG "v1.3.0"
        UPDATE_COMMAND ""
)
fetchcontent_makeavailable(DBITSET)

set_target_properties(dynamic_bitset PROPERTIES FOLDER "ThirdParty")
add_library(thirdparty::dynamic_bitset ALIAS dynamic_bitset)

