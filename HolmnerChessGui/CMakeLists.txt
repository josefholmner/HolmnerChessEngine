FILE(GLOB_RECURSE HEADERS "include/*.h")
FILE(GLOB SOURCES "src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(HolmnerChessGui ${HEADERS} ${SOURCES})

# Engine lib
include_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/include)
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/src)
target_link_libraries(HolmnerChessGui Engine)

# SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(HolmnerChessGui sfml-graphics)
