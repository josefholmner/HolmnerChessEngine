FILE(GLOB_RECURSE HEADERS "include/*.h")
FILE(GLOB_RECURSE THIRDPARTYWRAPPERHEADERS "include/ThirdPartyWrappers/*.h")
FILE(GLOB SOURCES "src/*.cpp" "src/ThirdPartyWrappers/*.cpp")
FILE(GLOB THIRDPARTYWRAPPERSOURCES "src/ThirdPartyWrappers/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(HolmnerChessGui ${HEADERS} ${THIRDPARTYWRAPPERHEADERS} ${SOURCES} ${THIRDPARTYWRAPPERSOURCES})

source_group("ThirdPartyWrappers" FILES ${THIRDPARTYWRAPPERHEADERS})
source_group("ThirdPartyWrappers" FILES ${THIRDPARTYWRAPPERSOURCES})

# Engine lib
include_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/include)
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/src)
target_link_libraries(HolmnerChessGui Engine)

# SFML
IF (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
ENDIF()
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(HolmnerChessGui sfml-graphics)
