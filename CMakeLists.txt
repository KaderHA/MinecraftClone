cmake_minimum_required(VERSION 3.23.1)
project(Tellus VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Minecraft)
endif()

add_subdirectory(${PROJECT_NAME})
add_subdirectory(Minecraft)
