FILE(GLOB_RECURSE HEADERS "include/*.h")
FILE(GLOB SOURCES "src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/include) # include files from Engine
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/src)

add_executable(HolmnerChessGui ${HEADERS} ${SOURCES})
target_link_libraries(HolmnerChessGui Engine)