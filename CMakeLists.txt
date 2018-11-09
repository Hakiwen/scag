cmake_minimum_required(VERSION 3.12)
project(SCAG)

set(CMAKE_CXX_STANDARD 17)

add_executable(SCAG main.cpp src/traveler_strategy.cpp src/traveler_strategy.h src/utils.h src/game.cpp src/game.h src/ambusher_strategy.cpp src/ambusher_strategy.h)