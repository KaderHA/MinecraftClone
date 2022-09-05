#include "RendererAPI.hpp"

#include <glad/glad.h>

namespace ts {

void RendererAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.b, color.g, color.a);
}
void RendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void RendererAPI::DrawIndexed(const Ref<VertexArray>& va) {
    glDrawElements(GL_TRIANGLES, va->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void RendererAPI::SetDepthFunc(unsigned int func) {
    glDepthFunc(func);
}

void RendererAPI::SetDepthMask(bool value) {
    if (value)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

};  // namespace ts