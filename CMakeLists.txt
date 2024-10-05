cmake_minimum_required(VERSION 3.10)
project(MyProject)

include_directories(lib)

link_directories(lib)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(main main.cpp ${SOURCES})

find_library(SQLITE3_LIBRARY sqlite3 lib)
find_library(RAYLIB_LIBRARY raylib lib)

target_link_libraries(main PUBLIC ${RAYLIB_LIBRARY} ${SQLITE3_LIBRARY})

add_subdirectory(src/Account)
add_subdirectory(src/Client)
add_subdirectory(src/Transaction)