file(GLOB SOURCES "src/*.cpp" "src/*.hpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory("ext/Tellus")

if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# add_custom_command(TARGET Game POST_BUILD        # Adds a post-build event to MyTest
# COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
# "$<TARGET_LINKER_FILE:${PROJECT_NAME}>"      # <--this is in-file
# $<TARGET_FILE_DIR:Game>)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/res COMMENT "Copying res to $<TARGET_FILE_DIR:${PROJECT_NAME}>")

message("-- Compiling using pre-compiled header")
add_compile_definitions(GLFW_INCLUDE_NONE)
target_link_libraries(${PROJECT_NAME} PRIVATE Tellus)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ext/Tellus/Tellus/src")