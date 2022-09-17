#include "Game.hpp"
#include "Skybox.hpp"
#include "ChunkManager.hpp"

static std::mutex ChunksMutex;

App::App() {
    PushLayer(new Game);
}
App::~App() {}

static ts::Ref<ts::TextureBuffer> CreateTexCoordBuffer(const ts::Ref<ts::Texture>& texture, unsigned int row_count, unsigned int column_count);

Game::Game() : Layer("GameLayer") {
    m_Camera = ts::Camera(glm::vec3(16.f, ((float)150), 16.f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.SetProjection(45.f, 1280.f / 720.f, 0.1f, 1000.f);

    // Skybox
    m_Skybox.reset(new Skybox("res/textures/sky"));
    m_SkyBoxShader.reset(new ts::Shader("res/shaders/skybox.vs", "res/shaders/skybox.fs"));

    // Terrain
    m_TerrainShader.reset(new ts::Shader("res/shaders/AOShader.vs", "res/shaders/AOShader.fs"));
    m_Texture.reset(new ts::Texture2D("res/textures/terrain.png"));
    m_TexCoordBuffer = CreateTexCoordBuffer(m_Texture, 16, 16);

    m_TerrainShader->Bind();
    m_TerrainShader->set1i("uTexture", 0);
    m_TerrainShader->set1i("uTexUV", 1);

    ts::Renderer::SetDepthMask(true);
}

Game::~Game() {}

void Game::OnUpdate(float dt) {
    m_Camera.OnUpdate(dt);
    ts::Renderer::BeginScene(m_Camera);

    m_TerrainShader->Bind();
    m_Texture->Bind();
    m_TexCoordBuffer->Bind(1);

    for (auto itr : ChunkManager::Chunks) {
        if (itr->GetVertexArray()->GetVertexCount() == 0) continue;
        m_TerrainShader->setMat4fv("uModel", itr->GetModelMatrix());
        ts::Renderer::Submit(itr->GetVertexArray(), m_TerrainShader);
    }

    ts::Renderer::SetDepthFunc(ts::DepthFunc::LEQUAL);
    m_SkyBoxShader->Bind();
    m_SkyBoxShader->setMat4fv("uProjection", m_Camera.GetProjection());
    m_SkyBoxShader->setMat4fv("uView", glm::mat4(glm::mat3(m_Camera.GetView())));
    m_Skybox->BindTexture(0);
    ts::Renderer::Submit(m_Skybox->GetVAO());
    ts::Renderer::SetDepthFunc(ts::DepthFunc::LESS);

    if (ts::Input::IsKeyPressed(TS_KEY_LEFT_SHIFT))
        m_Camera.SetSpeed(100.f);
    else
        m_Camera.SetSpeed(10.0f);

    ChunkManager::Update(m_Camera.GetPosition());
}

void Game::OnEvent(ts::Event& e) {
}

ts::Application* ts::CreateApplication() {
    return new App;
}

ts::Ref<ts::TextureBuffer> CreateTexCoordBuffer(const ts::Ref<ts::Texture>& texture, unsigned int row_count, unsigned int column_count) {
    float* uvArray = new float[((texture->GetWidth() / row_count) * (texture->GetHeight() / column_count)) * 8];
    const float spacing = (float)row_count / (float)texture->GetWidth();

    for (int y = column_count, index = 0; y > 0; y--) {
        for (int x = 0; x < row_count; x++, index += 8) {
            uvArray[index + 0] = (float)x * spacing;
            uvArray[index + 1] = (((float)y * spacing) - spacing);

            uvArray[index + 2] = ((float)x * spacing) + spacing;
            uvArray[index + 3] = (((float)y * spacing) - spacing);

            uvArray[index + 4] = ((float)x * spacing);
            uvArray[index + 5] = (float)y * spacing;

            uvArray[index + 6] = (float)x * spacing + spacing;
            uvArray[index + 7] = (float)y * spacing;
        }
    }

    ts::Ref<ts::TextureBuffer> tb(new ts::TextureBuffer(uvArray, (((texture->GetWidth() / row_count) * (texture->GetHeight() / column_count)) * 8) * sizeof(float), ts::TextureDataType::R32F));
    delete[] uvArray;
    return tb;
}