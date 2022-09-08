#include "Chunk.hpp"
#include "TextureAtlas.hpp"
#include "FastNoiseLite.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
};

Chunk::Chunk() : m_LocalChunkPosition(glm::vec3(0.f)) {
    m_Blocks = new Block[CHUNK_SIZE];
}

void Chunk::Init(glm::ivec3 localChunkPosition) {
    m_LocalChunkPosition = localChunkPosition;
    this->Generate();
}

Chunk::~Chunk() {
    delete[] m_Blocks;
}

void Chunk::Generate() {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
    noise.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
    noise.SetFractalOctaves(5);
    noise.SetFractalLacunarity(1.8f);

    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
                int height = (int)((noise.GetNoise((float)(x + m_LocalChunkPosition.x * CHUNK_WIDTH) / 2.f, (float)(z + m_LocalChunkPosition.z * CHUNK_DEPTH) / 2.f)) * (CHUNK_HEIGHT));

                if (y < height && y >= height - 10)
                    m_Blocks[index].SetBlockType(BlockType::Dirt);
                if (y < height - 10 && y > 0)
                    m_Blocks[index].SetBlockType(BlockType::Stone);
                if (y == height)
                    m_Blocks[index].SetBlockType(BlockType::Grass);
                if (y == 0)
                    m_Blocks[index].SetBlockType(BlockType::Bedrock);

                // Circle generation
                // int average = (CHUNK_WIDTH + CHUNK_HEIGHT + CHUNK_DEPTH) / 3;
                // if (sqrt((float)(x - CHUNK_WIDTH / 2) * (x - CHUNK_WIDTH / 2) + (y - CHUNK_HEIGHT / 2) * (y - CHUNK_HEIGHT / 2) + (z - CHUNK_DEPTH / 2) * (z - CHUNK_DEPTH / 2)) <= average / 2) {
                //     int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
                //     m_Blocks[index].SetBlockType(BlockType::Grass);
                // }
            }
        }
    }
}
void Chunk::CreateMesh(const ts::Ref<TextureAtlas>& texture) {
    Vertex* vertices = new Vertex[CHUNK_SIZE * 36];
    int block_vertex_index = 0;
    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
                if (m_Blocks[index].IsActive() == false) continue;
                glm::vec3 chunkPosition = {m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH};
                // Front
                TexCoords texCoords = texture->GetTextureCoords(m_Blocks[index].GetBlockType());
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z + 1.0f}, texCoords.side.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopLeft};

                // Back
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.side.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, texCoords.side.TopLeft};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, texCoords.side.TopLeft};

                // Right
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, texCoords.side.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopLeft};

                // Left
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z}, texCoords.side.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z + 1.0f}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.side.TopLeft};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z + 1.0f}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.side.TopLeft};

                // Top
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, texCoords.top.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.top.BottomRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.top.TopLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.top.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, texCoords.top.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.top.TopLeft};

                // Bottom

                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z}, texCoords.bottom.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.bottom.BottomRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z + 1.0f}, texCoords.bottom.TopLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.bottom.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, texCoords.bottom.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z + 1.0f}, texCoords.bottom.TopLeft};
            }
        }
    }

    float* float_vertices = new float[CHUNK_SIZE * 36 * 5];

    for (int i = 0; i < CHUNK_SIZE * 36; i++) {
        float_vertices[(i * 5) + 0] = vertices[i].Position.x;
        float_vertices[(i * 5) + 1] = vertices[i].Position.y;
        float_vertices[(i * 5) + 2] = vertices[i].Position.z;
        float_vertices[(i * 5) + 3] = vertices[i].TexCoord.x;
        float_vertices[(i * 5) + 4] = vertices[i].TexCoord.y;
    }

    ts::BufferLayout layout = {{0x1406, 3}, {0x1406, 2}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(float_vertices, (CHUNK_SIZE * 36 * 5 * sizeof(float)), layout));
    m_VertexArray.reset(new ts::VertexArray(vb));

    delete[] vertices;
    delete[] float_vertices;
}

// TextureFormat Chunk::GetUVs(BlockType type) {
// }