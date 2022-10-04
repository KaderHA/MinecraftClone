add_subdirectory(ext)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp" "ext/stb_image/*.cpp" "ext/stb_image/*.h")
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# if(MSVC)
# set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:DEBUG>:Debug>")
# endif()
add_compile_definitions(TS_ENABLE_ASSERTS GLFW_INCLUDE_NONE _CRT_SECURE_NO_WARNINGS)

if(MSVC)
    add_compile_definitions(TS_MSVC)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ext)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/tspch.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src "${CMAKE_CURRENT_SOURCE_DIR}/ext/stb_image")

# target_precompile_headers(${PROJECT_NAME} PRIVATE "include/${PROJECT_NAME}/tspch.hpp")
