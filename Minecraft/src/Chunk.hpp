#pragma once
#include <Tellus.hpp>

class Chunk {
   public:
    Chunk();
    ~Chunk();

    void Generate(int localChunkX, int localChunkY, int localChunkZ);
    void CreateMesh();

    const ts::Ref<ts::VertexArray>& GetVAO() const { return m_VertexArray; }

   public:
    static const int CHUNK_WIDTH = 4;
    static const int CHUNK_HEIGHT = 4;
    static const int CHUNK_DEPTH = 4;

   private:
    unsigned int* CreateIndices();

   private:
    ts::Ref<ts::VertexArray> m_VertexArray;
};