cmake_minimum_required(VERSION 3.16)
project(battleship)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
        "src/main.cpp"
        "src/players/*.cpp"
        "src/model/*.cpp"
        "src/game/*.cpp"
        "src/sql/*.cpp"
        "src/sql/DataManager.cpp"
)

add_executable(battleship ${SOURCES})
target_include_directories(battleship PRIVATE include)

target_link_libraries(battleship
        /opt/homebrew/lib/libpqxx.dylib
        /opt/homebrew/lib/postgresql@18/libpq.dylib
)