#include "Chunk.hpp"
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
void Chunk::CreateMesh() {
    unsigned int* vertices = new unsigned int[CHUNK_SIZE * 36];
    int block_vertex_index = 0;
    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
                if (m_Blocks[index].IsActive() == false) continue;
                glm::vec3 chunkPosition = {m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH};
                // Front
                TextureFormat blockFormat = this->GetUVs(m_Blocks[index].GetBlockType());

                // Front
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z + 1) << 12) | (0 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z + 1) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z + 1) << 12) | (3 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z + 1) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z + 1) << 12) | (2 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z + 1) << 12) | (3 << 18) | (blockFormat.side << 20);

                // Back
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z) << 12) | (0 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z) << 12) | (3 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z) << 12) | (2 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z) << 12) | (3 << 18) | (blockFormat.side << 20);

                // Right
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z + 1) << 12) | (0 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z + 1) << 12) | (3 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z) << 12) | (2 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z + 1) << 12) | (3 << 18) | (blockFormat.side << 20);

                // Left
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z) << 12) | (0 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z + 1) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z) << 12) | (3 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z + 1) << 12) | (1 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z + 1) << 12) | (2 << 18) | (blockFormat.side << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z) << 12) | (3 << 18) | (blockFormat.side << 20);

                // Top
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z + 1) << 12) | (0 << 18) | (blockFormat.top << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z + 1) << 12) | (1 << 18) | (blockFormat.top << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z) << 12) | (3 << 18) | (blockFormat.top << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z + 1) << 12) | (1 << 18) | (blockFormat.top << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y + 1) << 6) | ((z) << 12) | (2 << 18) | (blockFormat.top << 20);
                vertices[block_vertex_index++] = (x) | ((y + 1) << 6) | ((z) << 12) | (3 << 18) | (blockFormat.top << 20);

                // Bottom
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z) << 12) | (0 << 18) | (blockFormat.bottom << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z) << 12) | (1 << 18) | (blockFormat.bottom << 20);
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z + 1) << 12) | (3 << 18) | (blockFormat.bottom << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z) << 12) | (1 << 18) | (blockFormat.bottom << 20);
                vertices[block_vertex_index++] = (x + 1) | ((y) << 6) | ((z + 1) << 12) | (2 << 18) | (blockFormat.bottom << 20);
                vertices[block_vertex_index++] = (x) | ((y) << 6) | ((z + 1) << 12) | (3 << 18) | (blockFormat.bottom << 20);
            }
        }
    }

    ts::BufferLayout layout = {{0x1405, 1}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(vertices, (CHUNK_SIZE * 36 * sizeof(unsigned int)), layout));
    m_VertexArray.reset(new ts::VertexArray(vb));

    delete[] vertices;
}

TextureFormat Chunk::GetUVs(BlockType type) {
    TextureFormat tf;

    switch (type) {
        case BlockType::Grass:
            tf.top = 0;
            tf.side = 3;
            tf.bottom = 2;
            break;

        case BlockType::Dirt:
            tf.top = 2;
            tf.side = 2;
            tf.bottom = 2;
            break;
        case BlockType::Stone:
            tf.top = 1;
            tf.side = 1;
            tf.bottom = 1;
            break;
        case BlockType::Bedrock:
            tf.top = 17;
            tf.side = 17;
            tf.bottom = 17;
            break;
        default:
            break;
    }

    return tf;
}