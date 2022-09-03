#pragma once
#include "Core.hpp"

namespace ts {
class VertexArray;

class Camera;
class Shader;

class Renderer {
   public:
    static void BeginScene(const Camera& camera);
    static void EndScene();

    static void Submit(const Ref<VertexArray>& va);
    static void Submit(const Ref<VertexArray>& va, const Ref<Shader>& shader);
};

};  // namespace ts