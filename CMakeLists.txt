cmake_minimum_required(VERSION 3.16)
project(battleship)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(battleship ${SOURCES} include/Databases.h
        include/DatabaseConnection.h
        include/GameSaver.h
        src/sql/GameSaver.cpp)

target_include_directories(battleship PRIVATE
        include
        /opt/homebrew/include
        /opt/homebrew/opt/libpqxx/include
)

target_link_directories(battleship PRIVATE
        /opt/homebrew/lib
        /opt/homebrew/lib/postgresql@18
        /opt/homebrew/opt/libpqxx/lib
)

target_link_libraries(battleship pqxx pq)