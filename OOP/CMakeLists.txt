cmake_minimum_required(VERSION 2.8.9)
project(battleships)
include_directories(include)
set(SOURCES src/main.cpp src/Game.cpp src/Grid.cpp src/Player.cpp src/Ship.cpp)
add_executable(battleships_game ${SOURCES})