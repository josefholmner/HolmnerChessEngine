FILE(GLOB_RECURSE HEADERS "include/*.h")
FILE(GLOB SOURCES "src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(Tests ${HEADERS} ${SOURCES})


# Common lib
include_directories(${HolmnerChessEngine_SOURCE_DIR}/Common/include)
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Common/src)
target_link_libraries(Tests Common)

# Engine lib
include_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/include)
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/src)
target_link_libraries(Tests Engine)
