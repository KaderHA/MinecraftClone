#pragma once

namespace ts {

class Input {
   public:
    inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
    inline static bool IsMousePressed(int button) { return s_Instance->IsMousePressedImpl(button); }

    inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

   protected:
    virtual bool IsKeyPressedImpl(int keycode);
    virtual bool IsMousePressedImpl(int button);
    virtual std::pair<float, float> GetMousePositionImpl();

   private:
    static Input* s_Instance;
};

};  // namespace ts