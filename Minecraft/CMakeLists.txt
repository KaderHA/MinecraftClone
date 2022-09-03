file(GLOB SOURCES "src/*.cpp")
add_executable(Minecraft ${SOURCES})

# if(MSVC)
# set_property(TARGET Game PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:DEBUG>:Debug>")
# endif()

# add_custom_command(TARGET Game POST_BUILD        # Adds a post-build event to MyTest
# COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
# "$<TARGET_LINKER_FILE:${PROJECT_NAME}>"      # <--this is in-file
# $<TARGET_FILE_DIR:Game>)
add_custom_command(TARGET Minecraft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res/
    $<TARGET_FILE_DIR:Minecraft>/res COMMENT "Copying res to $<TARGET_FILE_DIR:Minecraft>")

message("-- Compiling using pre-compiled header")
add_compile_definitions(GLFW_INCLUDE_NONE)
target_link_libraries(Minecraft PRIVATE ${PROJECT_NAME})
target_include_directories(Minecraft PRIVATE ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src)