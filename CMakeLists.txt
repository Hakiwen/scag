cmake_minimum_required(VERSION 3.12)
project(SCAG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

find_package(ompl)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GUROBI REQUIRED)
add_executable(SCAG main.cpp src/traveler_strategy.cpp src/traveler_strategy.h src/utils.h src/game.cpp src/game.h src/ambusher_strategy.cpp src/ambusher_strategy.h src/game_map.cpp src/game_map.h src/square_obstacle.cpp src/square_obstacle.h src/point.cpp src/point.h src/simple_vertex.cpp src/simple_vertex.h)
include_directories(${GUROBI_INCLUDE_DIR})
target_link_libraries(SCAG ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GUROBI_LIBRARIES} gurobi80)
