fetchcontent_declare(
        PUGIXML
        GIT_REPOSITORY "https://github.com/zeux/pugixml.git"
        GIT_TAG "v1.14"
        UPDATE_COMMAND ""
)
fetchcontent_makeavailable(PUGIXML)

set_target_properties(pugixml PROPERTIES FOLDER "ThirdParty")
add_library(thirdparty::pugixml ALIAS pugixml)

