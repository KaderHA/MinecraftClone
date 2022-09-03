#include "Game.hpp"
#include "Cube.hpp"

App::App() {
    PushLayer(new Game);
}
App::~App() {}

Game::Game() : Layer("GameLayer") {
    ts::BufferLayout layout = {{0x1406, 3}, {0x1406, 2}};
    float* vertices = CreateChunk();
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(vertices, (SIZE * SIZE * SIZE * 24 * 5 * sizeof(float)), layout));
    ts::Ref<ts::IndexBuffer> ib(new ts::IndexBuffer(CreateIndices(), (SIZE * SIZE * SIZE * 36)));
    m_VertexArray.reset(new ts::VertexArray(vb, ib));

    std::string vSource = R"(
        #version 460 core

        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec2 aTexCoord; 
        uniform mat4 uViewProjection;

        out vec2 fTexCoord;

        void main() {
            gl_Position = uViewProjection * vec4(aPosition, 1.0);
            fTexCoord = aTexCoord;
        }
    )";

    std::string fSource = R"(
        #version 460 core

        in vec2 fTexCoord;
        uniform sampler2D uTexture;

        out vec4 Color;

        void main() {
            Color = texture(uTexture, fTexCoord);
        }
    )";

    m_Camera = ts::Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.SetProjection(45.f, 1280.f / 720.f, 0.1f, 1000.f);
    m_Shader.reset(new ts::Shader(vSource, fSource));

    m_Shader->Bind();
    m_Shader->set1i("uTexture", 0);

    m_Texture.reset(new ts::Texture2D("res/textures/Checkerboard.png"));
}

Game::~Game() {}

void Game::OnUpdate() {
    m_Camera.OnUpdate(.01f);
    ts::Renderer::BeginScene(m_Camera);
    m_Texture->Bind();
    ts::Renderer::Submit(m_VertexArray, m_Shader);
}

void Game::OnEvent(ts::Event& e) {
}

ts::Application* ts::CreateApplication() {
    return new App;
}