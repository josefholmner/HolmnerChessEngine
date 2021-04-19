FILE(GLOB_RECURSE HEADERS "include/*.h")
FILE(GLOB_RECURSE THIRDPARTYWRAPPERHEADERS "include/ThirdPartyWrappers/*.h")
FILE(GLOB SOURCES "src/*.cpp")
FILE(GLOB THIRDPARTYWRAPPERSOURCES "src/ThirdPartyWrappers/*.cpp")
FILE(GLOB RESOURCE_FILES "src/Resources/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(HolmnerChessGui ${HEADERS} ${THIRDPARTYWRAPPERHEADERS} ${SOURCES} ${THIRDPARTYWRAPPERSOURCES} ${RESOURCE_FILES})

source_group("ThirdPartyWrappers" FILES ${THIRDPARTYWRAPPERHEADERS})
source_group("ThirdPartyWrappers" FILES ${THIRDPARTYWRAPPERSOURCES})
source_group("Resources" FILES ${RESOURCE_FILES})

# Engine lib
include_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/include)
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Engine/src)
target_link_libraries(HolmnerChessGui Engine)

# Common lib
include_directories(${HolmnerChessEngine_SOURCE_DIR}/Common/include)
link_directories(${HolmnerChessEngine_SOURCE_DIR}/Common/src)
target_link_libraries(HolmnerChessGui Common)

# SFML
IF (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
ENDIF()
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(HolmnerChessGui sfml-graphics)
