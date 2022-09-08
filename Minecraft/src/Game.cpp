#include "Game.hpp"
#include "Skybox.hpp"

App::App() {
    PushLayer(new Game);
}
App::~App() {}

static ts::Ref<ts::TextureBuffer> CreateTexCoordBuffer(const ts::Ref<ts::Texture>& texture, unsigned int row_count, unsigned int column_count);

Game::Game() : Layer("GameLayer") {
    m_Camera = ts::Camera(glm::vec3(((float)Chunk::CHUNK_WIDTH / 2.f), ((float)Chunk::CHUNK_HEIGHT / 2.f), (float)Chunk::CHUNK_DEPTH * 2.f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.SetProjection(45.f, 1280.f / 720.f, 0.1f, 1000.f);

    // Skybox
    m_Skybox.reset(new Skybox("res/textures/sky"));
    m_SkyBoxShader.reset(new ts::Shader("res/shaders/skybox.vs", "res/shaders/skybox.fs"));

    // Terrain
    m_Shader.reset(new ts::Shader("res/shaders/default.vs", "res/shaders/default.fs"));
    m_Shader->Bind();
    m_Shader->set1i("uTexture", 0);
    m_Shader->set1i("uTexUV", 1);

    m_Texture.reset(new ts::Texture2D("res/textures/terrain.png"));
    m_TexCoordBuffer = CreateTexCoordBuffer(m_Texture, 16, 16);

    m_Chunks = new Chunk[16];
    for (int x = 0, i = 0; x < 4; x++) {
        for (int z = 0; z < 4; z++, i++) {
            m_Chunks[i].Init({x, 0.0f, z});
            m_Chunks[i].CreateMesh();
        }
    }

    // m_Chunk.reset(new Chunk);
    // m_Chunk->Generate();
    // m_Chunk->CreateMesh();
    ts::Renderer::SetDepthMask(true);
}

Game::~Game() {}

void Game::OnUpdate() {
    m_Camera.OnUpdate(.01f);
    ts::Renderer::BeginScene(m_Camera);

    m_Shader->Bind();
    m_Texture->Bind();
    m_TexCoordBuffer->Bind(1);
    // m_Shader->setMat4fv("uModel", m_Chunk->GetModelMatrix());
    // ts::Renderer::Submit(m_Chunk->GetVAO(), m_Shader);

    for (int x = 0, i = 0; x < 4; x++) {
        for (int z = 0; z < 4; z++, i++) {
            m_Shader->setMat4fv("uModel", m_Chunks[i].GetModelMatrix());
            ts::Renderer::Submit(m_Chunks[i].GetVAO(), m_Shader);
        }
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
        m_Camera.SetSpeed(1.0f);
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

            uvArray[index + 4] = ((float)x * spacing) + spacing;
            uvArray[index + 5] = (float)y * spacing;

            uvArray[index + 6] = (float)x * spacing;
            uvArray[index + 7] = (float)y * spacing;
        }
    }

    ts::Ref<ts::TextureBuffer> tb(new ts::TextureBuffer(uvArray, (((texture->GetWidth() / row_count) * (texture->GetHeight() / column_count)) * 8) * sizeof(float), ts::TextureDataType::R32F));
    delete[] uvArray;
    return tb;
}