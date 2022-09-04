#pragma once
#include <Tellus.hpp>
#include "Block.hpp"

class TextureAtlas;

class Chunk {
   public:
    Chunk(glm::ivec3 localChunkPosition = glm::ivec3(0));
    ~Chunk();

    void Generate();
    void CreateMesh(const ts::Ref<TextureAtlas>& texture);

    const ts::Ref<ts::VertexArray>& GetVAO() const { return m_VertexArray; }
    glm::mat4 GetModelMatrix() const { return glm::translate(glm::mat4(1.0f), m_LocalChunkPosition); }

   public:
    static const int CHUNK_WIDTH = 16;
    static const int CHUNK_HEIGHT = 16;
    static const int CHUNK_DEPTH = 16;
    static const int CHUNK_SIZE = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;

   private:
    unsigned int* CreateIndices();

   private:
    ts::Ref<ts::VertexArray> m_VertexArray;
    Block* m_Blocks;
    // Chunk(0,0), Chunk(1, 0) etc.
    glm::vec3 m_LocalChunkPosition;
};