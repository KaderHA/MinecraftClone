#include "Input.hpp"
#include "tspch.h"

#include <GLFW/glfw3.h>
#include "Application.hpp"

namespace ts {

Input* Input::s_Instance = new Input;

bool Input::IsKeyPressedImpl(int keycode) {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMousePressedImpl(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    int state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}
std::pair<float, float> Input::GetMousePositionImpl() {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return {(float)xPos, (float)yPos};
}

};  // namespace ts