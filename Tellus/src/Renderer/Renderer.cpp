#include "Renderer.hpp"
#include "RendererAPI.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>

namespace ts {

static glm::mat4 VP = glm::mat4(1.0f);

void Renderer::BeginScene(const Camera& camera) {
    VP = camera.GetViewProjection();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<VertexArray>& va) {
    va->Bind();
    RendererAPI::DrawIndexed(va);
}

void Renderer::Submit(const Ref<VertexArray>& va, const Ref<Shader>& shader) {
    shader->Bind();
    shader->setMat4fv("uViewProjection", VP);
    va->Bind();
    if (va->HasIndexBuffer())
        RendererAPI::DrawIndexed(va);
    else
        RendererAPI::DrawArray(va);
}
void Renderer::SetDepthFunc(DepthFunc func) {
    RendererAPI::SetDepthFunc(static_cast<unsigned int>(func));
}

void Renderer::SetDepthMask(bool value) {
    RendererAPI::SetDepthMask(value);
}

};  // namespace ts