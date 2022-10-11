#include "Application.hpp"
#include "tspch.h"

#include "Input/Input.hpp"
#include "Input/KeyCodes.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static bool MouseEnabled = false;

namespace ts {

Application* Application::s_Instance = nullptr;

Application::Application() {
    s_Instance = this;
    m_Window = Scope<Window>(new Window("Game", 1280, 720, WindowFlag::MOUSE_DISABLED));
    m_Window->SetCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {}

void Application::Run() {
    while (m_Running) {
        m_Window->OnUpdate();

        float time = m_Window->GetTime();
        float dt = time - m_LastFrame;
        m_LastFrame = time;

        // TODO -> Make client able to end the game loop
        if (Input::IsKeyPressed(TS_KEY_ESCAPE)) m_Running = false;

        /// TODO -> Find a more appropriate place
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CLIP_DISTANCE0);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (Layer* layer : m_LayerStack)
            layer->OnUpdate(dt);
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();
    }
}

void Application::OnEvent(Event& e) {
    // TODO -> Make client able to end the game loop
    if (e.GetEventType() == WindowClosedEvent::GetStaticType()) {
        m_Running = false;
    }

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.Handled) break;
    }
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* overlay) {
    m_LayerStack.PushOverlay(overlay);
}

}  // namespace ts