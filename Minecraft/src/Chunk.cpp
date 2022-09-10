#include "Chunk.hpp"
#include "FastNoiseLite.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
};

Chunk::Chunk() : m_Vertices(nullptr), m_VertexCount(0), m_LocalChunkPosition(glm::vec3(0.f)), m_Loaded(false), m_Uploaded(false) {
    m_Blocks = new Block[CHUNK_SIZE];
}

void Chunk::Init(glm::ivec3 localChunkPosition) {
    m_LocalChunkPosition = localChunkPosition;
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
                int height = (int)((noise.GetNoise((float)(x + m_LocalChunkPosition.x * CHUNK_WIDTH) / 2.f, (float)(z + m_LocalChunkPosition.z * CHUNK_DEPTH) / 2.f)) * (128)) + 64;
                int yHeight = y + (m_LocalChunkPosition.y * CHUNK_HEIGHT);

                if (yHeight < height && yHeight >= height - 10)
                    m_Blocks[index].SetBlockType(BlockType::Dirt);
                if (yHeight < height - 10 && yHeight > 0)
                    m_Blocks[index].SetBlockType(BlockType::Stone);
                if (yHeight == height)
                    m_Blocks[index].SetBlockType(BlockType::Grass);
                if (yHeight == 0)
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
    m_Vertices = new unsigned int[CHUNK_SIZE * 36];
    int block_vertex_index = 0;
    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
                if (m_Blocks[index].IsActive() == false) continue;
                glm::vec3 chunkPosition = {m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH};
                // Front
                TextureFormat format = this->GetUVs(m_Blocks[index].GetBlockType());

                bool lXNegative = false;
                if (x > 0) lXNegative = m_Blocks[index - 1].IsActive();
                bool lXPositive = false;
                if (x < CHUNK_WIDTH - 1) lXPositive = m_Blocks[index + 1].IsActive();
                bool lYNegative = false;
                if (y > 0) lYNegative = m_Blocks[index - CHUNK_WIDTH].IsActive();
                bool lYPositive = false;
                if (y < CHUNK_HEIGHT - 1) lYPositive = m_Blocks[index + CHUNK_WIDTH].IsActive();
                bool lZNegative = false;
                if (z > 0) lZNegative = m_Blocks[index - (CHUNK_HEIGHT * CHUNK_WIDTH)].IsActive();
                bool lZPositive = false;
                if (z < CHUNK_DEPTH - 1) lZPositive = m_Blocks[index + (CHUNK_HEIGHT * CHUNK_WIDTH)].IsActive();

                if (!lZPositive)
                    CreateFace(m_Vertices, block_vertex_index, {x, y, z}, format.side, BlockFace::Front);
                if (!lZNegative)
                    CreateFace(m_Vertices, block_vertex_index, {x, y, z}, format.side, BlockFace::Back);
                if (!lXPositive)
                    CreateFace(m_Vertices, block_vertex_index, {x, y, z}, format.side, BlockFace::Right);
                if (!lXNegative)
                    CreateFace(m_Vertices, block_vertex_index, {x, y, z}, format.side, BlockFace::Left);
                if (!lYPositive)
                    CreateFace(m_Vertices, block_vertex_index, {x, y, z}, format.top, BlockFace::Top);
                if (!lYNegative)
                    CreateFace(m_Vertices, block_vertex_index, {x, y, z}, format.bottom, BlockFace::Bottom);
            }
        }
    }

    m_VertexCount = block_vertex_index;
    m_Loaded = true;
}

void Chunk::CreateFace(unsigned int* vertices, int& index, glm::ivec3 pos, unsigned int format, BlockFace face) {
    switch (face) {
        case BlockFace::Front:
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (0 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (3 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (2 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (3 << 18) | (format << 20);
            break;
        case BlockFace::Back:
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z) << 12) | (0 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (3 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (2 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (3 << 18) | (format << 20);
            break;
        case BlockFace::Right:
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (0 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (3 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (2 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (3 << 18) | (format << 20);
            break;
        case BlockFace::Left:
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z) << 12) | (0 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (3 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (2 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (3 << 18) | (format << 20);
            break;
        case BlockFace::Top:
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (0 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (3 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z + 1) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (2 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y + 1) << 6) | ((pos.z) << 12) | (3 << 18) | (format << 20);
            break;
        case BlockFace::Bottom:
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z) << 12) | (0 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (3 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z) << 12) | (1 << 18) | (format << 20);
            vertices[index++] = (pos.x + 1) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (2 << 18) | (format << 20);
            vertices[index++] = (pos.x) | ((pos.y) << 6) | ((pos.z + 1) << 12) | (3 << 18) | (format << 20);
            break;
        default:
            TS_ASSERT(0, "No face given!");
            break;
    }
}

void Chunk::UploadToGPU() {
    ts::BufferLayout layout = {{0x1405, 1}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(m_Vertices, m_VertexCount * sizeof(unsigned int), layout));
    m_VertexArray.reset(new ts::VertexArray(vb));
    delete[] m_Vertices;
    m_Uploaded = true;
}

TextureFormat Chunk::GetUVs(BlockType type) {
    TextureFormat tf = {0 ,0, 0};

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