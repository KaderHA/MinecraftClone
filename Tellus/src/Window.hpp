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

    inline unsigned int GetWidth() const { return m_Data.Width; }
    inline unsigned int GetHeight() const { return m_Data.Height; }

    float GetTime() const;

    void SetCallback(const Callback& func);
    void* GetNativeWindow() { return m_Native; }

    void SetInputMode(unsigned int mode);

   private:
    GLFWwindow* m_Native;
    WindowData m_Data;
};

};  // namespace ts
