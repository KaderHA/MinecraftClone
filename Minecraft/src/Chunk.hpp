#pragma once
#include <Tellus.hpp>
#include "Block.hpp"

class Chunk {
   public:
    Chunk();
    ~Chunk();

    void Generate(int localChunkX = 0, int localChunkY = 0, int localChunkZ = 0);
    void CreateMesh();

    const ts::Ref<ts::VertexArray>& GetVAO() const { return m_VertexArray; }

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
};