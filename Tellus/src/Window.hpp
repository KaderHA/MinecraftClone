#pragma once
#include "Core.hpp"

#include "Events/Events.hpp"

struct GLFWwindow;

namespace ts {
using Callback = std::function<void(Event& e)>;

enum WindowFlag : unsigned int {
    MOUSE_DISABLED = 0x00034003,
};

struct WindowData {
    const char* Title;
    unsigned int Width, Height;
    bool VSync;

    Callback EventCallback;
};

class Window {
   public:
    Window(const char* title, int width, int height, unsigned int flags = 0);
    ~Window();

    void OnUpdate();

    inline unsigned int GetWidth() { return m_Data.Width; }
    inline unsigned int GetHeight() { return m_Data.Height; }

    void SetCallback(const Callback& func);
    void* GetNativeWindow() { return m_Native; }

   private:
    GLFWwindow* m_Native;
    WindowData m_Data;
};

};  // namespace ts
