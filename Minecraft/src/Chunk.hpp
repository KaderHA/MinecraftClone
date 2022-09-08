#pragma once
#include <Tellus.hpp>
#include "Block.hpp"

struct TextureFormat {
    unsigned int top;
    unsigned int side;
    unsigned int bottom;
};

class Chunk {
   public:
    Chunk();
    ~Chunk();

    void Init(glm::ivec3 localChunkPosition = glm::ivec3(0));

    void Generate();
    void CreateMesh();

    const ts::Ref<ts::VertexArray>& GetVAO() const { return m_VertexArray; }
    glm::mat4 GetModelMatrix() const { return glm::translate(glm::mat4(1.0f), {m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH}); }

   public:
    static const int CHUNK_WIDTH = 32;
    static const int CHUNK_HEIGHT = 32;
    static const int CHUNK_DEPTH = 32;
    static const int CHUNK_SIZE = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;

   private:
    TextureFormat GetUVs(BlockType type);

   private:
    ts::Ref<ts::VertexArray> m_VertexArray;
    Block* m_Blocks;
    // Chunk(0,0), Chunk(1, 0) etc.
    glm::vec3 m_LocalChunkPosition;
};