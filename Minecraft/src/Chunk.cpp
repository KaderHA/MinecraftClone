#include "Chunk.hpp"

#define WATER_LEVEL 120

/// Cube Indices ///
/**

           6---7
          /|  /|
         2---3 |
         | 4-|-5
         |/  |/
         0---1

**/
////////////////////

Chunk::Chunk() : m_Vertices(nullptr), m_VertexCount(0), m_LocalChunkPosition(glm::vec3(0.f)), m_Loaded(false) {
    m_Blocks.reset(new Block[CHUNK_SIZE]);
}

void Chunk::Init(glm::ivec3 localChunkPosition) {
    m_LocalChunkPosition = localChunkPosition;
}

Chunk::~Chunk() {}

using NeighborCallback = std::function<bool>(BlockFace face);

void Chunk::Generate() {
    float noise[(CHUNK_WIDTH) * (CHUNK_DEPTH)];
    // FastNoiseLite noise;
    // noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    // noise.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
    // noise.SetFractalOctaves(16);
    // noise.SetFractalLacunarity(2.f);
    // noise.SetFractalGain(0.5f);

    glm::ivec3 globPos(m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH);

    auto fnPerlin = FastNoise::New<FastNoise::Perlin>();
    auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

    fnFractal->SetSource(fnPerlin);
    fnFractal->SetOctaveCount(16);
    fnFractal->SetGain(0.5f);
    fnFractal->SetLacunarity(2.f);
    fnFractal->GenUniformGrid2D(noise, globPos.x, globPos.z, CHUNK_WIDTH, CHUNK_DEPTH, 0.005f, 1337);

    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                int index = ((z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x);
                int nIndex = (z * CHUNK_WIDTH) + x;
                float nHeight = (noise[nIndex] + 1.0f) / 2.f;

                int height = nHeight * 255;
                int yHeight = y + (m_LocalChunkPosition.y * CHUNK_HEIGHT);

                if (yHeight < height && yHeight >= height - 10)
                    m_Blocks[index].SetBlockType(BlockType::Dirt);
                else if (yHeight < height - 10 && yHeight > 0)
                    m_Blocks[index].SetBlockType(BlockType::Stone);
                else if (yHeight == height)
                    m_Blocks[index].SetBlockType(BlockType::Grass);
                else if (yHeight == 0)
                    m_Blocks[index].SetBlockType(BlockType::Bedrock);

                if (height <= WATER_LEVEL && yHeight == WATER_LEVEL) {
                    m_Blocks[index].SetBlockType(BlockType::Water);
                }
            }
        }
    }
}
void Chunk::CreateMesh() {
    m_Vertices = new unsigned int[CHUNK_SIZE * 36];
    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
                if (m_Blocks[index].IsActive() == false) continue;
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
                    CreateFace(format.side,
                               glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1), glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1));
                if (!lZNegative)
                    CreateFace(format.side,
                               glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z), glm::ivec3(x + 1, y + 1, z), glm::ivec3(x, y + 1, z));
                if (!lXPositive)
                    CreateFace(format.side,
                               glm::ivec3(x + 1, y, z + 1), glm::ivec3(x + 1, y, z), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z));
                if (!lXNegative)
                    CreateFace(format.side,
                               glm::ivec3(x, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x, y + 1, z + 1));
                if (!lYPositive)
                    CreateFace(format.top,
                               glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x + 1, y + 1, z));
                if (!lYNegative)
                    CreateFace(format.bottom,
                               glm::ivec3(x, y, z), glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1));
            }
        }
    }
    m_Loaded = true;
}

void Chunk::CreateFace(unsigned int format, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11) {
    m_Vertices[m_VertexCount++] = (pos00.x) | ((pos00.y) << 6) | ((pos00.z) << 12) | (0 << 18) | (format << 20);
    m_Vertices[m_VertexCount++] = (pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (format << 20);
    m_Vertices[m_VertexCount++] = (pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (format << 20);

    m_Vertices[m_VertexCount++] = (pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (format << 20);
    m_Vertices[m_VertexCount++] = (pos11.x) | ((pos11.y) << 6) | ((pos11.z) << 12) | (3 << 18) | (format << 20);
    m_Vertices[m_VertexCount++] = (pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (format << 20);
}

void Chunk::UploadToGPU() {
    ts::BufferLayout layout = {{0x1405, 1}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(m_Vertices, m_VertexCount * sizeof(unsigned int), layout));
    m_VertexArray.reset(new ts::VertexArray(vb));
    delete[] m_Vertices;
}

TextureFormat Chunk::GetUVs(BlockType type) {
    TextureFormat tf = {0, 0, 0};

    switch (type) {
        case BlockType::Grass:
            tf.top = 0;
            tf.side = 3;
            tf.bottom = 2;
            break;

        case BlockType::Dirt:
            tf = {2, 2, 2};
            break;

        case BlockType::Stone:
            tf = {1, 1, 1};
            break;

        case BlockType::Bedrock:
            tf = {17, 17, 17};
            break;

        case BlockType::Water:
            tf = {205, 205, 205};
    }

    return tf;
}