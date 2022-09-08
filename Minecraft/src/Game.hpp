#pragma once
#define TS_MAIN
#include <Tellus.hpp>
#include "Chunk.hpp"

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const glm::ivec3& position) const {
        return (position.x * 88339) ^ (position.z * 91967) ^ (position.z * 126323);
    }
};

class Skybox;

class Game : public ts::Layer {
   public:
    Game();
    ~Game();

    void OnUpdate() override;
    void OnEvent(ts::Event& event) override;

   private:
    // std::unordered_map<glm::ivec3, Chunk, ChunkPositionHash> m_ChunkMap;
    Chunk* m_Chunks;
    ts::Scope<Chunk> m_Chunk;
    ts::Ref<ts::VertexArray> m_VertexArray;
    ts::Camera m_Camera;
    ts::Ref<ts::Shader> m_Shader;

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