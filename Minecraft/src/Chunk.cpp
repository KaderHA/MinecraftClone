#include "Chunk.hpp"
#include "ChunkManager.hpp"

#define ISO_SURFACE 0.31
#define FREQUENCY 0.0022f

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

    srand(m_LocalChunkPosition.x + m_LocalChunkPosition.y);
    m_Fractal->GenUniformGrid2D(noise, globPos.x, globPos.z, CHUNK_WIDTH, CHUNK_DEPTH, FREQUENCY, 1337);

    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            int noiseIdx = (z * CHUNK_WIDTH) + x;

            int height = ((noise[noiseIdx] + 1.0f) / 2.f) * 255;
            bool tree = rand() % 200 == 199 && height % 32 <= 31 - 6;

            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
                int yHeight = y + (m_LocalChunkPosition.y * CHUNK_HEIGHT);

                if (tree && height > WATER_LEVEL && ProperDistance(glm::ivec3(x, y, z), 2)) {
                    if ((yHeight > height && yHeight <= height + 3)) {
                        m_Blocks[index].SetBlockType(BlockType::Log);
                        if (yHeight == height + 3) {
                            m_Blocks[index + 1].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - 1].SetBlockType(BlockType::Leaf);
                            m_Blocks[index + (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                        }
                    } else if ((yHeight > height + 3 && yHeight <= height + 6)) {
                        m_Blocks[index].SetBlockType(BlockType::Leaf);
                        m_Blocks[index + 1].SetBlockType(BlockType::Leaf);
                        m_Blocks[index - 1].SetBlockType(BlockType::Leaf);

                        m_Blocks[index + (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                        m_Blocks[index - (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);

                        if (yHeight == height + 4) {
                            m_Blocks[index + 2].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - 2].SetBlockType(BlockType::Leaf);

                            m_Blocks[index + 2 + (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index + 2 - (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - 2 + (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - 2 - (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);

                            m_Blocks[index + ((CHUNK_WIDTH * CHUNK_HEIGHT) * 2)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - ((CHUNK_WIDTH * CHUNK_HEIGHT) * 2)].SetBlockType(BlockType::Leaf);

                            m_Blocks[index + ((CHUNK_WIDTH * CHUNK_HEIGHT) * 2) + 1].SetBlockType(BlockType::Leaf);
                            m_Blocks[index + ((CHUNK_WIDTH * CHUNK_HEIGHT) * 2) - 1].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - ((CHUNK_WIDTH * CHUNK_HEIGHT) * 2) + 1].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - ((CHUNK_WIDTH * CHUNK_HEIGHT) * 2) - 1].SetBlockType(BlockType::Leaf);
                        }
                        if (yHeight == height + 6) {
                        } else {
                            m_Blocks[index + 1 + (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - 1 + (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index + 1 - (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                            m_Blocks[index - 1 - (CHUNK_WIDTH * CHUNK_HEIGHT)].SetBlockType(BlockType::Leaf);
                        }
                    }
                }

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
                } else if (height < WATER_LEVEL && yHeight >= height - 5 && yHeight <= height)
                    m_Blocks[index].SetBlockType(BlockType::Sand);
            }
        }
    }
}

void Chunk::CreateMesh() {
    m_Vertices = new unsigned int[CHUNK_SIZE * 36];
    int STEP_X = 1, STEP_Y = CHUNK_WIDTH, STEP_Z = CHUNK_WIDTH * CHUNK_HEIGHT;
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

                if (!NeighborActive(index, STEP_Z))  // Forward
                    AddQuadAO(format.side, index, STEP_Z, STEP_X, STEP_Y, glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1), glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1));
                if (!NeighborActive(index, -STEP_Z))  // Backward
                    AddQuadAO(format.side, index, -STEP_Z, -STEP_X, STEP_Y, glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z), glm::ivec3(x + 1, y + 1, z), glm::ivec3(x, y + 1, z));
                if (!NeighborActive(index, STEP_X))  // Right
                    AddQuadAO(format.side, index, STEP_X, -STEP_Z, STEP_Y, glm::ivec3(x + 1, y, z + 1), glm::ivec3(x + 1, y, z), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z));
                if (!NeighborActive(index, -STEP_X))  // Left
                    AddQuadAO(format.side, index, -STEP_X, STEP_Z, STEP_Y, glm::ivec3(x, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x, y + 1, z + 1));
                if (!NeighborActive(index, STEP_Y))  // Up
                    AddQuadAO(format.top, index, STEP_Y, STEP_X, -STEP_Z, glm::ivec3(x, y + 1, z + 1), glm::ivec3(x + 1, y + 1, z + 1), glm::ivec3(x, y + 1, z), glm::ivec3(x + 1, y + 1, z));
                if (!NeighborActive(index, -STEP_Y))  // Down
                    AddQuadAO(format.bottom, index, -STEP_Y, STEP_X, STEP_Z, glm::ivec3(x, y, z), glm::ivec3(x + 1, y, z), glm::ivec3(x, y, z + 1), glm::ivec3(x + 1, y, z + 1));
            }
        }
    }
    m_Loaded = true;
    m_Altered = false;
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

void Chunk::AddQuadAO(unsigned int format, int32_t idx, int32_t facingOffset, int32_t offsetA, int32_t offsetB, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11) {
    int32_t facingIdx = idx + facingOffset;

    unsigned int sideA0 = NeighborActive(idx, facingOffset, -offsetA);
    unsigned int sideA1 = NeighborActive(idx, facingOffset, +offsetA);
    unsigned int sideB0 = NeighborActive(idx, facingOffset, -offsetB);
    unsigned int sideB1 = NeighborActive(idx, facingOffset, +offsetB);

    unsigned int corner00 = (sideA0 & sideB0) || NeighborActive(idx, facingOffset, -offsetA, -offsetB);
    unsigned int corner01 = (sideA0 & sideB1) || NeighborActive(idx, facingOffset, -offsetA, +offsetB);
    unsigned int corner10 = (sideA1 & sideB0) || NeighborActive(idx, facingOffset, +offsetA, -offsetB);
    unsigned int corner11 = (sideA1 & sideB1) || NeighborActive(idx, facingOffset, +offsetA, +offsetB);

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
    m_Fractal->GenUniformGrid3D(noise, globPos.x - 1, globPos.y - 1, globPos.z - 1, SIZE_GEN, SIZE_GEN, SIZE_GEN, 0.005f, 1337);

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

    float height = ((m_Fractal->GenSingle2D((globPos.x + blockPos.x) * FREQUENCY, (globPos.z + blockPos.z) * FREQUENCY, 1337) + 1.0f) / 2.f) * 255;
    int yHeight = blockPos.y + globPos.y;

    if (yHeight <= height)
        return true;
    else
        return false;
}

bool Chunk::NeighborActive(int index, int offsetA, int offsetB, int offsetC) {
    bool NeighborChunk = false;
    glm::ivec3 idx = To3D(index);
    glm::ivec3 off = To3D(offsetA) + To3D(offsetB) + To3D(offsetC);

    glm::ivec3 chunkPos = m_LocalChunkPosition;
    if ((idx.z == 0 && off.z < 0) || (idx.z == CHUNK_DEPTH - 1 && off.z > 0)) {
        chunkPos.z += off.z;
        idx += off;
        idx.z = off.z > 0 ? 0 : CHUNK_DEPTH - 1;
        NeighborChunk = true;
    }
    if ((idx.y == 0 && off.y < 0) || (idx.y == CHUNK_HEIGHT - 1 && off.y > 0)) {
        chunkPos.y += off.y;
        idx += off;
        idx.y = off.y > 0 ? 0 : CHUNK_HEIGHT - 1;
        NeighborChunk = true;
    }
    if ((idx.x == 0 && off.x < 0) || (idx.x == CHUNK_WIDTH - 1 && off.x > 0)) {
        chunkPos.x += off.x;
        idx += off;
        idx.x = off.x > 0 ? 0 : CHUNK_WIDTH - 1;
        NeighborChunk = true;
    }
    if (NeighborChunk)
        return NeighborActive(chunkPos, idx);
    else
        return m_Blocks[index + (offsetA + offsetB + offsetC)].IsActive() && !m_Blocks[index + (offsetA + offsetB + offsetC)].IsTransparent();
}

bool Chunk::ProperDistance(glm::ivec3 blockPos, int maxDist) {
    if (blockPos.x >= CHUNK_WIDTH - maxDist || blockPos.x < maxDist ||
        // blockPos.y >= CHUNK_HEIGHT - maxDist || blockPos.y < maxDist ||
        blockPos.z >= CHUNK_DEPTH - maxDist || blockPos.z < maxDist)
        return false;

    return true;
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
            break;
        case BlockType::Sand:
            tf = {18, 18, 18};
            break;
        case BlockType::Gravel:
            tf = {19, 19, 19};
            break;
        case BlockType::Log:
            tf = {21, 20, 21};
            break;
        case BlockType::Leaf:
            tf = {52, 52, 52};
            break;
    }

    return tf;
}

glm::ivec3 Chunk::To3D(int index) {
    int z = index / (CHUNK_WIDTH * CHUNK_HEIGHT);
    index -= z * CHUNK_WIDTH * CHUNK_HEIGHT;
    int y = index / CHUNK_WIDTH;
    int x = index % CHUNK_WIDTH;
    return glm::ivec3(x, y, z);
}