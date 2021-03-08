FILE(GLOB_RECURSE PUBLIC_HEADERS "include/Engine/*.h")
FILE(GLOB SOURCES "src/*.cpp")
FILE(GLOB_RECURSE PRIVATE_HEADERS "src/PrivateInclude/*.h")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_LIBRARY(Engine ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

source_group("PrivateInclude" FILES ${PRIVATE_HEADERS})


# Common lib
include_directories(${HolmnerChessEngine_SOURCE_DIR}/Common/include)
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Common/src)
target_link_libraries(Engine Common)
