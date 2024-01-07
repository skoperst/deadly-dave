project ("deadly-dave")

# One should find their SDL installation path of SDL2 library
set(SDL2 "C:\users\nuc1\library\SDL2-2.28.5")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${deadly-dave_SOURCE_DIR}/")

include_directories(include)
include_directories(${SDL2}/${SDL2_ARCH}/include)

link_directories(${SDL2}/lib/x64)
if (WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")# Add source to this project's executable.
  add_executable (deadly-dave WIN32 "main.c" "game.c" "invfreq.c" "monster.c" "plasma.c" "tile.c" "dave.c" "soundfx.c" "bullet.c")
endif()



target_link_libraries(deadly-dave SDL2)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET deadly-dave PROPERTY CXX_STANDARD 20)
endif()
