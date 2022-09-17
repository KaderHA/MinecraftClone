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
    bool OnKeyPressed(ts::KeyPressedEvent& event);

   private:
    ts::Camera m_Camera;
    ts::Ref<ts::Shader> m_TerrainShader;

    ts::Ref<ts::Texture2D> m_Texture;
    ts::Ref<ts::TextureBuffer> m_TexCoordBuffer;

    ts::Ref<Skybox> m_Skybox;
    ts::Ref<ts::Shader> m_SkyBoxShader;
};

class App : public ts::Application {
   public:
    App();
    ~App();
};