#include "Game.hpp"
#include "Skybox.hpp"

#define CHUNK_RADIUS 8

static std::mutex ChunksMutex;

App::App() {
    PushLayer(new Game);
}
App::~App() {}

static ts::Ref<ts::TextureBuffer> CreateTexCoordBuffer(const ts::Ref<ts::Texture>& texture, unsigned int row_count, unsigned int column_count);

Game::Game() : Layer("GameLayer") {
    m_Camera = ts::Camera(glm::vec3(((float)Chunk::CHUNK_WIDTH / 2.f), ((float)256), (float)Chunk::CHUNK_DEPTH * 2.f), glm::vec3(0.0f, 1.0f, 0.0f));
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

    /*for (int z = 0; z < 4; z++) {
        for (int x = 0; x < 4; x++) {
            ts::Ref<Chunk> chunk(new Chunk);
            chunk->Init({ x, 0, z });
            chunk->CreateMesh();
            m_Chunks.push_back(chunk);
        }
    }*/

    // m_Chunk.reset(new Chunk);
    // m_Chunk->Generate();
    // m_Chunk->CreateMesh();
    ts::Renderer::SetDepthMask(true);
    // m_Futures.push_back(std::async(std::launch::async, Game::ChunkWorker, &m_ChunkLoadList, &m_LoadChunks));
}

Game::~Game() {
    m_LoadChunks = false;
}

void Game::OnUpdate(float dt) {
    m_Camera.OnUpdate(dt);
    ts::Renderer::BeginScene(m_Camera);

    m_Shader->Bind();
    m_Texture->Bind();
    m_TexCoordBuffer->Bind(1);

    for (auto itr : m_Chunks) {
        m_Shader->setMat4fv("uModel", itr->GetModelMatrix());
        ts::Renderer::Submit(itr->GetVertexArray(), m_Shader);
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

    // LoadChunks(m_Camera.GetCameraPosition());
    if (ts::Input::IsKeyPressed(TS_KEY_DELETE) && m_Chunks.size() > 0) {
        m_Chunks.pop_back();
    }

    UnloadChunks();
    LoadChunks();
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

void Game::ChunkWorker(std::vector<ts::Ref<Chunk>>* loadlist, bool* load) {
    while (*load) {
        std::lock_guard<std::mutex> lock(ChunksMutex);
        for (int i = 0; i < loadlist->size(); i++) {
            if (!loadlist->at(i)->IsLoaded()) {
                glm::vec3 pos = loadlist->at(i)->GetPosition();
                // std::cout << "Chunk: {" << pos.x << ", " << pos.y << ", " << pos.z << "} Loaded\n";
                loadlist->at(i)->Generate();
                loadlist->at(i)->CreateMesh();
            }
            loadlist->clear();
        }
    }
}

void Game::LoadChunks() {
    int startZ = (m_Camera.GetPosition().z / Chunk::CHUNK_DEPTH) - CHUNK_RADIUS;
    int endZ = (m_Camera.GetPosition().z / Chunk::CHUNK_DEPTH) + CHUNK_RADIUS;

    int endY = 256 / Chunk::CHUNK_HEIGHT;

    int startX = (m_Camera.GetPosition().x / Chunk::CHUNK_WIDTH) - CHUNK_RADIUS;
    int endX = (m_Camera.GetPosition().x / Chunk::CHUNK_WIDTH) + CHUNK_RADIUS;

    for (int z = startZ; z < endZ; z++) {
        for (int y = 0; y < endY; y++) {
            for (int x = startX; x < endX; x++) {
                glm::vec3 pos(x, y, z);
                if (std::find_if(m_Chunks.begin(), m_Chunks.end(), [pos](ts::Ref<Chunk> chunk) { return chunk->GetPosition() == pos; }) == m_Chunks.end()) {
                    // std::cout << "Chunk: {" << x << ", " << y << ", " << z << "} Loaded\n";
                    ts::Ref<Chunk> chunk(new Chunk);
                    chunk->Init({x, y, z});
                    chunk->Generate();
                    chunk->CreateMesh();
                    chunk->UploadToGPU();
                    m_Chunks.push_back(chunk);
                    return;
                }
            }
        }
    }
}

void Game::UnloadChunks() {
    for (int i = 0; i < m_Chunks.size(); ++i) {
        glm::vec2 camChunkCoords(m_Camera.GetPosition().x / Chunk::CHUNK_WIDTH, m_Camera.GetPosition().z / Chunk::CHUNK_DEPTH);
        glm::ivec2 chunkSpace = {m_Chunks[i]->GetPosition().x - camChunkCoords.x, m_Chunks[i]->GetPosition().z - camChunkCoords.y};

        if (abs(chunkSpace.x) > CHUNK_RADIUS || abs(chunkSpace.y) > CHUNK_RADIUS) {
            m_Chunks.erase(std::remove(m_Chunks.begin(), m_Chunks.end(), m_Chunks[i]), m_Chunks.end());
        }
    }
}

void Game::UploadToGPU() {
    for (auto itr : m_Chunks) {
        if (itr->IsLoaded() && !itr->IsUploaded()) {
            itr->UploadToGPU();
        }
    }
}