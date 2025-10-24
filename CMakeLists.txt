cmake_minimum_required(VERSION 4.0)
project(battleship)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(battleship ${SOURCES} ${HEADERS})
target_include_directories(battleship PRIVATE include)




