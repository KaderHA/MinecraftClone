#include "Game.hpp"
#include "Skybox.hpp"
#include "ChunkManager.hpp"

static std::mutex ChunksMutex;
static ts::Ref<ts::TextureBuffer> CreateTexCoordBuffer(const ts::Ref<ts::Texture>& texture, unsigned int row_count, unsigned int column_count);

App::App() { PushLayer(new Game); }
App::~App() {}

Game::Game() : Layer("GameLayer") {}
Game::~Game() {}

void Game::OnAttach() {
    m_Camera = ts::Camera(glm::vec3(16.f, ((float)150), 16.f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.SetProjection(45.f, 1280.f / 720.f, 0.1f, 1000.f);

    // Skybox
    m_Skybox.reset(new Skybox("res/textures/sky"));
    m_SkyBoxShader.reset(new ts::Shader("res/shaders/skybox.vs", "res/shaders/skybox.fs"));

    // Terrain
    m_TerrainShader.reset(new ts::Shader("res/shaders/terrain.vs", "res/shaders/terrain.fs"));
    m_Texture.reset(new ts::Texture2D("res/textures/terrain.png"));
    m_TexCoordBuffer = CreateTexCoordBuffer(m_Texture, 16, 16);

    m_TerrainShader->Bind();
    m_TerrainShader->set1i("uTexture", 0);
    m_TerrainShader->set1i("uTexUV", 1);

    // Water
    m_WaterShader.reset(new ts::Shader("res/shaders/water.vs", "res/shaders/water.fs"));
    m_WaterShader->Bind();
    m_WaterShader->set1i("uTexture", 0);
    m_WaterShader->set1i("uTexUV", 1);
    m_WaterShader->set1i("uReflectionTexture", 2);
    m_WaterShader->set1i("uRefractionTexture", 3);

    m_Reflection.reset(new ts::FrameBuffer(320, 180));
    m_Reflection->AddColorTexture();
    m_Reflection->AddDepthRenderBuffer();
    if (!m_Reflection->Check()) {
        std::cout << "FrameBuffer is not complete!" << '\n';
    }
    m_Refraction.reset(new ts::FrameBuffer(1280, 720));
    m_Refraction->AddColorTexture();
    m_Refraction->AddDepthRenderBuffer();
    if (!m_Refraction->Check()) {
        std::cout << "FrameBuffer is not complete!" << '\n';
    }

    ts::Renderer::SetDepthMask(true);
}

void Game::OnDetach() {}

void Game::OnUpdate(float dt) {
    m_Camera.OnUpdate(dt);
    float dist = 2 * (m_Camera.GetPosition().y - (WATER_LEVEL + 1));
    m_Camera.SetPosition(m_Camera.GetPosition().x, m_Camera.GetPosition().y - dist, m_Camera.GetPosition().z);
    m_Camera.InversePitch();
    ts::Renderer::BeginScene(m_Camera);

    m_Reflection->Bind();
    RenderScene(glm::vec4(0, 1, 0, -WATER_LEVEL));
    m_Reflection->Unbind();

    m_Camera.SetPosition(m_Camera.GetPosition().x, m_Camera.GetPosition().y + dist, m_Camera.GetPosition().z);
    m_Camera.InversePitch();
    ts::Renderer::BeginScene(m_Camera);

    m_Refraction->Bind();
    RenderScene(glm::vec4(0, -1, 0, WATER_LEVEL + 1));
    m_Refraction->Unbind();
    RenderScene();

    m_WaterShader->Bind();
    m_Texture->Bind(0);
    m_TexCoordBuffer->Bind(1);
    m_Reflection->BindColorTexture(2);
    m_Refraction->BindColorTexture(3);

    for (const auto& itr : ChunkManager::Chunks) {
        if (itr->GetWaterVertexArray()->GetVertexCount() == 0) continue;
        m_WaterShader->setMat4fv("uModel", itr->GetModelMatrix());
        ts::Renderer::Submit(itr->GetWaterVertexArray(), m_WaterShader);
    }

    if (ts::Input::IsKeyPressed(TS_KEY_LEFT_SHIFT))
        m_Camera.SetSpeed(100.f);
    else
        m_Camera.SetSpeed(10.0f);

    ChunkManager::Update(m_Camera.GetPosition());
}

void Game::RenderScene(const glm::vec4& clipPlane) {
    ts::Renderer::Clear(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

    m_TerrainShader->Bind();
    m_Texture->Bind(0);
    m_TexCoordBuffer->Bind(1);

    for (const auto& itr : ChunkManager::Chunks) {
        if (itr->GetTerrainVertexArray()->GetVertexCount() == 0) continue;
        m_TerrainShader->setMat4fv("uModel", itr->GetModelMatrix());
        m_TerrainShader->setVec4fv("uPlane", clipPlane);
        ts::Renderer::Submit(itr->GetTerrainVertexArray(), m_TerrainShader);
    }

    ts::Renderer::SetDepthFunc(ts::DepthFunc::LEQUAL);
    m_SkyBoxShader->Bind();
    m_SkyBoxShader->setMat4fv("uProjection", m_Camera.GetProjection());
    m_SkyBoxShader->setMat4fv("uView", glm::mat4(glm::mat3(m_Camera.GetView())));
    m_Skybox->BindTexture(0);
    ts::Renderer::Submit(m_Skybox->GetVAO());
    ts::Renderer::SetDepthFunc(ts::DepthFunc::LESS);
}

void Game::OnEvent(ts::Event& event) {
    // ts::EventDispatcher dispatcher(event);
    // dispatcher.Dispatch<ts::KeyPressedEvent>(std::bind(&Game::OnKeyPressed, this, std::placeholders::_1));
}

bool Game::OnKeyPressed(ts::KeyPressedEvent& event) {
    return true;
}

void Game::OnImGuiRender() {
    // ImGui::Text("m_Reflection:");
    // ImGui::Text("Pointer = %p", m_Reflection->GetColorTexture(0));
    // ImGui::Text("size = %d x %d", m_Reflection->GetWidth(), m_Reflection->GetHeight());
    // ImGui::Image((void*)(intptr_t)m_Reflection->GetColorTexture(0), ImVec2(m_Reflection->GetWidth(), m_Reflection->GetWidth()));
    // ImGui::Text("Refraction:");
    // ImGui::Text("Pointer = %p", m_Refraction->GetColorTexture(0));
    // ImGui::Text("size = %d x %d", m_Refraction->GetWidth(), m_Refraction->GetHeight());
    // ImGui::Image((void*)(intptr_t)m_Refraction->GetColorTexture(0), ImVec2(m_Refraction->GetWidth(), m_Refraction->GetWidth()));
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