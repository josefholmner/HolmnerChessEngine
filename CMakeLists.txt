cmake_minimum_required(VERSION 3.16.3)
set (CMAKE_CXX_STANDARD 17)

if (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)

project(HolmnerChessEngine)

add_subdirectory(HolmnerChessGui)
add_subdirectory(Engine)
add_subdirectory(Tests)
add_subdirectory(Common)
