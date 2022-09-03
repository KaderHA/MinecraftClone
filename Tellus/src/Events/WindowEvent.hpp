#pragma once
#include "Event.hpp"

namespace ts {

class WindowResizedEvent : public Event {
   public:
    WindowResizedEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

    inline unsigned int GetWidth() const { return m_Width; }
    inline unsigned int GetHeight() const { return m_Height; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResized);

   private:
    unsigned int m_Width, m_Height;
};

class WindowClosedEvent : public Event {
   public:
    WindowClosedEvent() {}

    EVENT_CLASS_TYPE(WindowClosed);
};
};  // namespace ts