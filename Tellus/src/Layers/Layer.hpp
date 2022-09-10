#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"

namespace ts {

class Layer {
   public:
    Layer(const char* name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnEvent(Event& event) {}

    inline const char* GetName() const { return m_DebugName; }

   protected:
    const char* m_DebugName;
};

};  // namespace ts