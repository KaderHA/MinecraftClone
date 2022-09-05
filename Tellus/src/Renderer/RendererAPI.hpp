#pragma once
#include "Core.hpp"

#include <glm/glm.hpp>
#include "Buffer.hpp"

namespace ts {

class RendererAPI {
   public:
    static void SetClearColor(const glm::vec4& color);
    static void Clear();
    static void DrawIndexed(const Ref<VertexArray>& va);

    static void SetDepthMask(bool value);
};

};  // namespace ts