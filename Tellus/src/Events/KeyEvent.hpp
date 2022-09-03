#pragma once
#include "Event.hpp"

namespace ts {

class KeyEvent : public Event {
   public:
    inline int GetKeyCode() const { return m_KeyCode; }

   protected:
    KeyEvent(int keycode) : m_KeyCode(keycode) {}

    int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
   public:
    KeyPressedEvent(int keycode, int repeatcount) : KeyEvent(keycode), m_RepeatCount(repeatcount) {}
    inline int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeat)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed);

   private:
    int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent {
   public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_KeyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased);
};

};  // namespace ts
