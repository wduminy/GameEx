cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("Sneaky")
set(SFML_ROOT_DIR "./" 
    CACHE PATH "Path to sfml")
set(BOX2D_ROOT_DIR "./" 
    CACHE PATH "Path to BOX2D")    
add_executable(sneaky 
command.cpp
exception.cpp
flaremap.cpp
game.cpp
gamestate.cpp
gui.cpp
log.cpp
physics.cpp
scenenode.cpp
sneakyworld.cpp
sneaky_main.cpp
wang2edge.cpp)
set_property(TARGET sneaky APPEND_STRING PROPERTY COMPILE_FLAGS "-std=c++0x -fmessage-length=0")    
include_directories(${SFML_ROOT_DIR}/include ${BOX2D_ROOT_DIR}/include) 
link_directories(${SFML_ROOT_DIR}/lib ${BOX2D_ROOT_DIR}/lib)
find_library(sfml-system sfml-system ${SFML_ROOT_DIR}/lib)
find_library(sfml-graphics sfml-graphics ${SFML_ROOT_DIR}/lib)
find_library(sfml-audio sfml-audio ${SFML_ROOT_DIR}/lib)
find_library(sfml-window sfml-window ${SFML_ROOT_DIR}/lib)
find_library(Box2D Box2D ${BOX2D_ROOT_DIR}/lib)
target_link_libraries(sneaky ${sfml-audio} ${sfml-window} ${sfml-system} ${sfml-graphics} ${Box2D})

