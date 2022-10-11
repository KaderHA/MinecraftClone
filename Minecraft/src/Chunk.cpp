#include "Chunk.hpp"
#include "ChunkManager.hpp"

#define ISO_SURFACE 0.0

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
    auto fnPerlin = FastNoise::New<FastNoise::Perlin>();
    m_Fractal = FastNoise::New<FastNoise::FractalFBm>();
    m_Fractal->SetSource(fnPerlin);
    m_Fractal->SetOctaveCount(16);
    m_Fractal->SetGain(0.5f);
    m_Fractal->SetLacunarity(2.f);
}

Chunk::~Chunk() {}

void Chunk::Generate() {
    float noise[(CHUNK_WIDTH) * (CHUNK_DEPTH)];
    glm::ivec3 globPos(m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH);

    m_Fractal->GenUniformGrid2D(noise, globPos.x, globPos.z, CHUNK_WIDTH, CHUNK_DEPTH, 0.005f, 1337);

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

                if (height < WATER_LEVEL && yHeight == WATER_LEVEL && yHeight > height) {
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
                if (m_Blocks[index].GetBlockType() == BlockType::Water) {
                    TextureFormat format = this->GetUVs(m_Blocks[index].GetBlockType());
                    CreateWaterFace(format.top, glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x + 1, y + 1, z));
                    continue;
                }
                if (m_Blocks[index].IsActive() == false) continue;
                TextureFormat format = this->GetUVs(m_Blocks[index].GetBlockType());

                bool lXNegative = false;
                if (x > 0)
                    lXNegative = m_Blocks[index - 1].IsActive();
                else
                    lXNegative = NeighborActive(glm::ivec3(m_LocalChunkPosition.x - 1, m_LocalChunkPosition.y, m_LocalChunkPosition.z), glm::ivec3(CHUNK_WIDTH - 1, y, z));

                bool lXPositive = false;
                if (x < CHUNK_WIDTH - 1)
                    lXPositive = m_Blocks[index + 1].IsActive();
                else
                    lXPositive = NeighborActive(glm::ivec3(m_LocalChunkPosition.x + 1, m_LocalChunkPosition.y, m_LocalChunkPosition.z), glm::ivec3(0, y, z));

                bool lYNegative = false;
                if (y > 0)
                    lYNegative = m_Blocks[index - CHUNK_WIDTH].IsActive();
                else
                    lYNegative = NeighborActive(glm::ivec3(m_LocalChunkPosition.x, m_LocalChunkPosition.y - 1, m_LocalChunkPosition.z), glm::ivec3(x, CHUNK_HEIGHT - 1, z));

                bool lYPositive = false;
                if (y < CHUNK_HEIGHT - 1)
                    lYPositive = m_Blocks[index + CHUNK_WIDTH].IsActive();
                else
                    lYPositive = NeighborActive(glm::ivec3(m_LocalChunkPosition.x, m_LocalChunkPosition.y + 1, m_LocalChunkPosition.z), glm::ivec3(x, 0, z));

                bool lZNegative = false;
                if (z > 0)
                    lZNegative = m_Blocks[index - (CHUNK_HEIGHT * CHUNK_WIDTH)].IsActive();
                else
                    lZNegative = NeighborActive(glm::ivec3(m_LocalChunkPosition.x, m_LocalChunkPosition.y, m_LocalChunkPosition.z - 1), glm::ivec3(x, y, CHUNK_DEPTH - 1));

                bool lZPositive = false;
                if (z < CHUNK_DEPTH - 1)
                    lZPositive = m_Blocks[index + (CHUNK_HEIGHT * CHUNK_WIDTH)].IsActive();
                else
                    lZPositive = NeighborActive(glm::ivec3(m_LocalChunkPosition.x, m_LocalChunkPosition.y, m_LocalChunkPosition.z + 1), glm::ivec3(x, y, 0));

                if (!lZPositive)
                    CreateFace(format.side, glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1), glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1));
                if (!lZNegative)
                    CreateFace(format.side, glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z), glm::ivec3(x + 1, y + 1, z), glm::ivec3(x, y + 1, z));
                if (!lXPositive)
                    CreateFace(format.side, glm::ivec3(x + 1, y, z + 1), glm::ivec3(x + 1, y, z), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z));
                if (!lXNegative)
                    CreateFace(format.side, glm::ivec3(x, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x, y + 1, z + 1));
                if (!lYPositive)
                    CreateFace(format.top, glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x + 1, y + 1, z));
                if (!lYNegative)
                    CreateFace(format.bottom, glm::ivec3(x, y, z), glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1));
            }
        }
    }
    m_Loaded = true;
}

void Chunk::CreateFace(unsigned int format, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11) {
    m_Vertices[m_VertexCount++] = (pos00.x) | ((pos00.y) << 6) | ((pos00.z) << 12) | (0 << 18) | (0 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (0 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (0 << 20) | (format << 22);

    m_Vertices[m_VertexCount++] = (pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (0 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos11.x) | ((pos11.y) << 6) | ((pos11.z) << 12) | (3 << 18) | (0 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (0 << 20) | (format << 22);
}

void Chunk::CreateWaterFace(unsigned int format, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11) {
    m_WaterVertices.push_back((pos00.x) | ((pos00.y) << 6) | ((pos00.z) << 12) | (0 << 18) | (0 << 20) | (format << 22));
    m_WaterVertices.push_back((pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (0 << 20) | (format << 22));
    m_WaterVertices.push_back((pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (0 << 20) | (format << 22));

    m_WaterVertices.push_back((pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (0 << 20) | (format << 22));
    m_WaterVertices.push_back((pos11.x) | ((pos11.y) << 6) | ((pos11.z) << 12) | (3 << 18) | (0 << 20) | (format << 22));
    m_WaterVertices.push_back((pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (0 << 20) | (format << 22));
}

void Chunk::GenMesh() {
    m_Vertices = new unsigned int[CHUNK_SIZE * 36];
    constexpr int32_t SIZE_GEN = 32 + 2;
    float noise[SIZE_GEN * SIZE_GEN * SIZE_GEN];
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
    fnFractal->GenUniformGrid3D(noise, globPos.x - 1, globPos.y - 1, globPos.z - 1, SIZE_GEN, SIZE_GEN, SIZE_GEN, 0.005f, 1337);

    constexpr int32_t STEP_X = 1;
    constexpr int32_t STEP_Y = SIZE_GEN;
    constexpr int32_t STEP_Z = SIZE_GEN * SIZE_GEN;

    int32_t noiseIdx = STEP_X + STEP_Y + STEP_Z;

    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                if (noise[noiseIdx] > ISO_SURFACE) {
                    noiseIdx++;
                    continue;
                }
                TextureFormat format = this->GetUVs(BlockType::Stone);

                if (noise[noiseIdx + STEP_X] > ISO_SURFACE)  // Right
                    AddQuadAO(noise, format.side, noiseIdx, STEP_X, -STEP_Z, STEP_Y, glm::ivec3(x + 1, y, z + 1), glm::ivec3(x + 1, y, z), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z));

                if (noise[noiseIdx - STEP_X] > ISO_SURFACE)  // Left
                    AddQuadAO(noise, format.side, noiseIdx, -STEP_X, STEP_Z, STEP_Y, glm::ivec3(x, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x, y + 1, z + 1));

                if (noise[noiseIdx + STEP_Y] > ISO_SURFACE)  // Up
                    AddQuadAO(noise, format.top, noiseIdx, STEP_Y, STEP_X, -STEP_Z, glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x + 1, y + 1, z));

                if (noise[noiseIdx - STEP_Y] > ISO_SURFACE)  // Down
                    AddQuadAO(noise, format.bottom, noiseIdx, -STEP_Y, STEP_X, STEP_Z, glm::ivec3(x, y, z), glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1));

                if (noise[noiseIdx + STEP_Z] > ISO_SURFACE)  // Forward
                    AddQuadAO(noise, format.side, noiseIdx, STEP_Z, STEP_X, STEP_Y, glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1), glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1));

                if (noise[noiseIdx - STEP_Z] > ISO_SURFACE)  // Back
                    AddQuadAO(noise, format.side, noiseIdx, -STEP_Z, -STEP_X, STEP_Y, glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z), glm::ivec3(x + 1, y + 1, z), glm::ivec3(x, y + 1, z));

                noiseIdx++;
            }
            noiseIdx += STEP_X * 2;
        }
        noiseIdx += STEP_Y * 2;
    }
}

void Chunk::AddQuadAO(float* noise, unsigned int format, int32_t idx, int32_t facingOffset, int32_t offsetA, int32_t offsetB, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11) {
    int32_t facingIdx = idx + facingOffset;

    unsigned int sideA0 = noise[facingIdx - offsetA] <= ISO_SURFACE;
    unsigned int sideA1 = noise[facingIdx + offsetA] <= ISO_SURFACE;
    unsigned int sideB0 = noise[facingIdx - offsetB] <= ISO_SURFACE;
    unsigned int sideB1 = noise[facingIdx + offsetB] <= ISO_SURFACE;

    unsigned int corner00 = (sideA0 & sideB0) || noise[facingIdx - offsetA - offsetB] <= ISO_SURFACE;
    unsigned int corner01 = (sideA0 & sideB1) || noise[facingIdx - offsetA + offsetB] <= ISO_SURFACE;
    unsigned int corner10 = (sideA1 & sideB0) || noise[facingIdx + offsetA - offsetB] <= ISO_SURFACE;
    unsigned int corner11 = (sideA1 & sideB1) || noise[facingIdx + offsetA + offsetB] <= ISO_SURFACE;

    unsigned int ao00 = sideA0 + sideB0 + corner00;
    unsigned int ao10 = sideA1 + sideB0 + corner10;
    unsigned int ao01 = sideA0 + sideB1 + corner01;
    unsigned int ao11 = sideA1 + sideB1 + corner11;

    m_Vertices[m_VertexCount++] = (pos00.x) | ((pos00.y) << 6) | ((pos00.z) << 12) | (0 << 18) | (ao00 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (ao10 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (ao01 << 20) | (format << 22);

    m_Vertices[m_VertexCount++] = (pos10.x) | ((pos10.y) << 6) | ((pos10.z) << 12) | (1 << 18) | (ao10 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos11.x) | ((pos11.y) << 6) | ((pos11.z) << 12) | (3 << 18) | (ao11 << 20) | (format << 22);
    m_Vertices[m_VertexCount++] = (pos01.x) | ((pos01.y) << 6) | ((pos01.z) << 12) | (2 << 18) | (ao01 << 20) | (format << 22);
}

void Chunk::UploadToGPU() {
    ts::BufferLayout layout = {{0x1405, 1}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(m_Vertices, m_VertexCount * sizeof(unsigned int), layout));
    m_TerrainVA.reset(new ts::VertexArray(vb));
    ts::Ref<ts::VertexBuffer> wvb(new ts::VertexBuffer(m_WaterVertices.data(), m_WaterVertices.size() * sizeof(unsigned int), layout));
    m_WaterVA.reset(new ts::VertexArray(wvb));
    delete[] m_Vertices;
    m_WaterVertices.clear();
}

bool Chunk::NeighborActive(glm::ivec3 chunkPos, glm::ivec3 blockPos) {
    glm::ivec3 globPos(chunkPos.x * CHUNK_WIDTH, chunkPos.y * CHUNK_HEIGHT, chunkPos.z * CHUNK_DEPTH);

    float height = ((m_Fractal->GenSingle2D((globPos.x + blockPos.x) * 0.005f, (globPos.z + blockPos.z) * 0.005f, 1337) + 1.0f) / 2.f) * 255;
    int yHeight = blockPos.y + (chunkPos.y * CHUNK_HEIGHT);

    if (yHeight <= height)
        return true;
    else
        return false;
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