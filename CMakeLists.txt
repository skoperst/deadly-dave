cmake_minimum_required(VERSION 3.20)
project ("deadly-dave")

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
find_package(OpenGL REQUIRED)


#==============================================================
#          SDL2
#=============================================================
set( SDL2_DISABLE_SDL2MAIN ON CACHE INTERNAL "Disable SDL2Main")
set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL
#    GIT_TAG         SDL2
    GIT_TAG         origin/release-2.28.x
    GIT_SHALLOW     TRUE
#    GIT_PROGRESS    TRUE
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(SDL2)

message(STATUS "SDL2 Include Dir: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 Source Dir: ${SDL2_SOURCE_DIR}")
message(STATUS "SDL2 Binary Dir: ${SDL2_BINARY_DIR}")
#=========================================================================


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${deadly-dave_SOURCE_DIR}/")
set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON)

include_directories(include)

if (WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")# Add source to this project's executable.
endif()


 add_executable (deadly-dave "main.c" "game.c" "logz.c" "invfreq.c" "monster.c" "plasma.c" "tile.c" "dave.c" "soundfx.c" "bullet.c")

target_link_libraries(deadly-dave PUBLIC SDL2::SDL2-static)
#set_property(TARGET deadly-dave)
