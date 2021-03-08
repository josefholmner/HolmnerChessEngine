cmake_minimum_required(VERSION 3.16.3)
set (CMAKE_CXX_STANDARD 17)

project(HolmnerChessEngine)

add_subdirectory(HolmnerChessGui)
add_subdirectory(Engine)
add_subdirectory(Tests)
add_subdirectory(Common)
