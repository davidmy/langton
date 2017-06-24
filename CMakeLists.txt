cmake_minimum_required (VERSION 2.8.11)
project(langton)

find_package(sdl2 REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "..." FORCE)
endif()

set(SOURCES
  "${SRC_DIR}/main.cpp"
  "${SRC_DIR}/grid.h"
  "${SRC_DIR}/grid.cpp"
  "${SRC_DIR}/ant.h"
  "${SRC_DIR}/ant.cpp"
  "${SRC_DIR}/button.h"
  "${SRC_DIR}/button.cpp"
  "${SRC_DIR}/gui.h"
  "${SRC_DIR}/gui.cpp"
  "${SRC_DIR}/simulation.h"
  "${SRC_DIR}/simulation.cpp"
  "${SRC_DIR}/loadedsimulation.h"
  "${SRC_DIR}/loadedsimulation.cpp"
)

add_executable(langton ${SOURCES})

target_link_libraries(langton
  ${SDL2_LIBRARIES}
  jsoncpp
)

install(TARGETS langton DESTINATION .)
install(DIRECTORY data DESTINATION .)
install(FILES langton.json DESTINATION .)
