add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)

set_target_properties(glfw PROPERTIES FOLDER "ext/GLFW3")
set_target_properties(uninstall PROPERTIES FOLDER "ext/GLFW3")
set_target_properties(update_mappings PROPERTIES FOLDER "ext/GLFW3")
set_target_properties(glad PROPERTIES FOLDER "ext")

add_library(ext INTERFACE)
target_link_libraries(ext INTERFACE glad glfw glm::glm)
