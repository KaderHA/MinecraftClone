add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(FastNoise2)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ImGui)
add_library(ImGui STATIC)
target_sources(ImGui
    PRIVATE
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp

    PRIVATE
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

target_include_directories(ImGui
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
)

set_target_properties(glfw PROPERTIES FOLDER "ext/GLFW3")
set_target_properties(uninstall PROPERTIES FOLDER "ext/GLFW3")
set_target_properties(update_mappings PROPERTIES FOLDER "ext/GLFW3")
set_target_properties(glad PROPERTIES FOLDER "ext")
set_target_properties(FastNoise PROPERTIES FOLDEr "ext")

add_library(ext INTERFACE)
target_link_libraries(ext INTERFACE glad glfw glm::glm ImGui FastNoise)
