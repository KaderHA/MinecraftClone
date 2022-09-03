#pragma once
#include "Event.hpp"

namespace ts {
class MouseMovedEvent : public Event {
   public:
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    inline float GetX() const { return m_MouseX; }
    inline float GetY() const { return m_MouseY; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved);

   private:
    float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event {
   public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    inline float GetXOffset() const { return m_XOffset; }
    inline float GetYOffset() const { return m_YOffset; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled);

   private:
    float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event {
   public:
    inline int GetButton() const { return m_Button; }

   protected:
    MouseButtonEvent(int button) : m_Button(button) {}

    int m_Button;
};

class MousePressedEvent : public MouseButtonEvent {
   public:
    MousePressedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MousePressedEvent: " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MousePressed);
};

class MouseReleasedEvent : public MouseButtonEvent {
   public:
    MouseReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseReleasedEvent: " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseReleased);
};

};  // namespace ts