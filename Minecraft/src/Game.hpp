#pragma once
#define TS_MAIN
#include <Tellus.hpp>
#include "Chunk.hpp"

class Skybox;

class Game : public ts::Layer {
   public:
    Game();
    ~Game();

    void OnUpdate(float dt) override;
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(ts::Event& event) override;
    bool OnMousePressed(ts::MousePressedEvent& event);
    virtual void OnImGuiRender() override;

    void RenderScene(const glm::vec4& clipPlane = glm::vec4(0));

   private:
    ts::Camera m_Camera;
    ts::Ref<ts::Shader> m_TerrainShader, m_WaterShader;

    ts::Ref<ts::Texture2D> m_Texture;
    ts::Ref<ts::TextureBuffer> m_TexCoordBuffer;
    ts::Ref<ts::FrameBuffer> m_Reflection, m_Refraction;

    ts::Ref<Skybox> m_Skybox;
    ts::Ref<ts::Shader> m_SkyBoxShader;
};

class App : public ts::Application {
   public:
    App();
    ~App();
};