#include "Game.hpp"

App::App() {
    PushLayer(new Game);
}
App::~App() {}

Game::Game() : Layer("GameLayer") {
    m_Camera = ts::Camera(glm::vec3(((float)Chunk::CHUNK_WIDTH / 2.f), ((float)Chunk::CHUNK_HEIGHT / 2.f), (float)Chunk::CHUNK_DEPTH * 2.f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.SetProjection(45.f, 1280.f / 720.f, 0.1f, 1000.f);
    m_Shader.reset(new ts::Shader("res/shaders/default.vs", "res/shaders/default.fs"));

    m_Shader->Bind();
    m_Shader->set1i("uTexture", 0);

    m_Texture.reset(new ts::Texture2D("res/textures/terrain.png"));
    m_WorldTexture.reset(new TextureAtlas(m_Texture, 16, 16));

    m_Chunk.reset(new Chunk);
    m_Chunk->Generate();
    m_Chunk->CreateMesh(m_WorldTexture);
}

Game::~Game() {}

void Game::OnUpdate() {
    m_Camera.OnUpdate(.01f);
    ts::Renderer::BeginScene(m_Camera);
    m_Texture->Bind();
    m_Shader->setMat4fv("uModel", m_Chunk->GetModelMatrix());
    ts::Renderer::Submit(m_Chunk->GetVAO(), m_Shader);

    if (ts::Input::IsKeyPressed(TS_KEY_LEFT_SHIFT)) m_Camera.SetSpeed(10.f);
}

void Game::OnEvent(ts::Event& e) {
}

ts::Application* ts::CreateApplication() {
    return new App;
}