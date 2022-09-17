#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "tspch.h"
#include "Input/Input.hpp"

namespace ts {

Window::Window(const char* title, int width, int height, unsigned int flags) {
    m_Data.Title = title;
    m_Data.Width = width;
    m_Data.Height = height;
    int success = glfwInit();
    TS_ASSERT(success, "GLFW failed to initialize.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Native = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title, nullptr, nullptr);
    glfwMakeContextCurrent(m_Native);

    if (flags & WindowFlag::MOUSE_DISABLED)
        glfwSetInputMode(m_Native, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    TS_ASSERT(success, "Glad failed to initialize.");
    glViewport(0, 0, m_Data.Width, m_Data.Height);

    glfwSetWindowUserPointer(m_Native, &m_Data);

    // Callbacks
    glfwSetWindowSizeCallback(m_Native, [](GLFWwindow* window, int width, int height) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
        data->Width = width;
        data->Height = height;

        WindowResizedEvent evt(width, height);
        data->EventCallback(evt);
    });

    glfwSetWindowCloseCallback(m_Native, [](GLFWwindow* window) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        WindowClosedEvent evt;
        data->EventCallback(evt);
    });

    glfwSetKeyCallback(m_Native, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                data->EventCallback(event);
                break;
            }

            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data->EventCallback(event);
                break;
            }

            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                data->EventCallback(event);
                break;
            }
        };
    });

    glfwSetMouseButtonCallback(m_Native, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS: {
                MousePressedEvent event(button);
                data->EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseReleasedEvent event(button);
                data->EventCallback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_Native, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data->EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Native, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data->EventCallback(event);
    });

    // glfwSetWindowCloseCallback(m_Native, [](GLFWwindow* window) {
    //     WindowData* w = (WindowData*)glfwGetWindowUserPointer(window);

    //     WindowClosedEvent e;
    //     w->EventCallback(e);
    // });
}

Window::~Window() {}

void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Native);
    glViewport(0, 0, m_Data.Width, m_Data.Height);
}

float Window::GetTime() const {
    return glfwGetTime();
}

void Window::SetCallback(const Callback& func) {
    m_Data.EventCallback = func;
}

void Window::SetInputMode(unsigned int mode) {
    glfwSetInputMode(m_Native, GLFW_CURSOR, mode);
}

}  // namespace ts
