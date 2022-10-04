#pragma once
#include "Core.hpp"

namespace ts {
class VertexArray;

class Camera;
class Shader;

enum class DepthFunc {
    LESS = 0x0201,
    EQUAL = 0x0202,
    LEQUAL = 0x0203,
    GREATER = 0x0204,
    NOTEQUAL = 0x0205,
    GEQUAL = 0x0206,
    ALWAYS = 0x0207
};

class Renderer {
   public:
    static void BeginScene(const Camera& camera);
    static void EndScene();

    static void Submit(const Ref<VertexArray>& va);
    static void Submit(const Ref<VertexArray>& va, const Ref<Shader>& shader);

    static void SetDepthFunc(DepthFunc func);
    static void SetDepthMask(bool value);
};

};  // namespace ts