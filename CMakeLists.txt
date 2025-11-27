cmake_minimum_required(VERSION 3.16)
project(battleship)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
        "src/main.cpp"
        "src/players/*.cpp"
        "src/model/*.cpp"
        "src/game/*.cpp"
)
file(GLOB HEADERS "include/*.h")

add_executable(battleship ${SOURCES} ${HEADERS})
target_include_directories(battleship PRIVATE include)