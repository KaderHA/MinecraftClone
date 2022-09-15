#pragma once
#include <Tellus.hpp>
#include "Block.hpp"

struct TextureFormat {
    unsigned int top;
    unsigned int side;
    unsigned int bottom;
};

enum class BlockFace {
    Front,
    Back,
    Right,
    Left,
    Top,
    Bottom
};

class Chunk {
   public:
    Chunk();
    ~Chunk();

    void Init(glm::ivec3 localChunkPosition = glm::ivec3(0));

    bool operator==(const Chunk& other) { return m_LocalChunkPosition == other.GetPosition(); }
    bool operator!=(const Chunk& other) { return m_LocalChunkPosition != other.GetPosition(); }

    void Generate();
    void CreateMesh();
    void UploadToGPU();

    const ts::Ref<ts::VertexArray>& GetVertexArray() const { return m_VertexArray; }
    glm::mat4 GetModelMatrix() const { return glm::translate(glm::mat4(1.0f), {m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH}); }
    glm::ivec3 GetPosition() const { return m_LocalChunkPosition; }

    inline bool IsLoaded() const { return m_Loaded; }

    void Chunk::CreateFace(unsigned int format, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11);

   public:
    static const int CHUNK_WIDTH = 32;
    static const int CHUNK_HEIGHT = 32;
    static const int CHUNK_DEPTH = 32;
    static const int CHUNK_SIZE = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;

   private:
    TextureFormat GetUVs(BlockType type);

   private:
    unsigned int* m_Vertices;
    bool m_Loaded;
    int m_VertexCount;
    ts::Ref<ts::VertexArray> m_VertexArray;
    ts::Scope<Block[]> m_Blocks;
    // Chunk(0,0), Chunk(1, 0) etc.
    glm::ivec3 m_LocalChunkPosition;
};