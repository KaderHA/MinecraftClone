#include "Game.hpp"
#include "Skybox.hpp"

App::App() {
    PushLayer(new Game);
}
App::~App() {}

Game::Game() : Layer("GameLayer") {
    for (int i = 0; i < 6; i++) {
        skyboxIndices[i * 6 + 0] = 0 + i * 4;
        skyboxIndices[i * 6 + 1] = 1 + i * 4;
        skyboxIndices[i * 6 + 2] = 3 + i * 4;
        skyboxIndices[i * 6 + 3] = 1 + i * 4;
        skyboxIndices[i * 6 + 4] = 2 + i * 4;
        skyboxIndices[i * 6 + 5] = 3 + i * 4;
    }

    m_Camera = ts::Camera();
    // m_Camera = ts::Camera(glm::vec3(((float)Chunk::CHUNK_WIDTH / 2.f), ((float)Chunk::CHUNK_HEIGHT / 2.f), (float)Chunk::CHUNK_DEPTH * 2.f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.SetProjection(45.f, 1280.f / 720.f, 0.1f, 1000.f);
    m_Shader.reset(new ts::Shader("res/shaders/default.vs", "res/shaders/default.fs"));

    ts::BufferLayout skyboxLayout = {{0x1406, 3}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(skyboxVertices, sizeof(skyboxVertices), skyboxLayout));
    ts::Ref<ts::IndexBuffer> ib(new ts::IndexBuffer(skyboxIndices, 36));
    m_SkyBox.reset(new ts::VertexArray(vb, ib));
    m_SkyBoxShader.reset(new ts::Shader("res/shaders/skybox.vs", "res/shaders/skybox.fs"));

    m_Shader->Bind();
    m_Shader->set1i("uTexture", 0);

    m_Texture.reset(new ts::Texture2D("res/textures/terrain.png"));
    m_WorldTexture.reset(new TextureAtlas(m_Texture, 16, 16));

    m_SkyBoxTexture.reset(new ts::TextureCubeMap("res/textures/sky"));

    m_Chunk.reset(new Chunk);
    m_Chunk->Generate();
    m_Chunk->CreateMesh(m_WorldTexture);
}

Game::~Game() {}

void Game::OnUpdate() {
    m_Camera.OnUpdate(.01f);
    ts::Renderer::BeginScene(m_Camera);

    ts::Renderer::SetDepthMask(false);
    m_SkyBoxShader->Bind();
    m_SkyBoxShader->setMat4fv("uProjection", m_Camera.GetProjection());
    m_SkyBoxShader->setMat4fv("uView", glm::mat4(glm::mat3(m_Camera.GetView())));
    m_SkyBoxTexture->Bind();
    ts::Renderer::Submit(m_SkyBox);
    ts::Renderer::SetDepthMask(true);

    m_Shader->Bind();
    m_Shader->setMat4fv("uModel", m_Chunk->GetModelMatrix());
    m_Texture->Bind();
    ts::Renderer::Submit(m_Chunk->GetVAO(), m_Shader);

    if (ts::Input::IsKeyPressed(TS_KEY_LEFT_SHIFT)) m_Camera.SetSpeed(10.f);
}

void Game::OnEvent(ts::Event& e) {
}

ts::Application* ts::CreateApplication() {
    return new App;
}