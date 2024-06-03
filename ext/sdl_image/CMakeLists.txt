fetchcontent_declare(
        SDL_IMAGE
        URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2.zip
        UPDATE_COMMAND ""
)

fetchcontent_makeavailable(SDL_IMAGE)
add_library(thirdparty::sdl2_image ALIAS SDL2_image)
#set_target_properties(SDL2_TTF PROPERTIES FOLDER "ThirdParty")


