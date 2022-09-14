#include "ChunkManager.hpp"
#include "Chunk.hpp"

#define CHUNK_RADIUS 2
#define CHUNKS_PER_FRAME 2

std::vector<ts::Ref<Chunk>> ChunkManager::Chunks;
std::unordered_set<ChunkRegister, ChunkPositionHash> ChunkManager::m_ChunkRegister;
// Multithreaded
std::queue<std::future<ts::Ref<Chunk>>> ChunkManager::m_LoadList;

// SingleThread
// std::vector<ts::Ref<Chunk>> ChunkManager::m_LoadList;

void ChunkManager::Update(glm::vec3 cameraPosition) {
    LoadChunks(cameraPosition);
    CheckChunks();
    UnloadChunks(cameraPosition);
    SynchronizeChunks();
}

void ChunkManager::LoadChunks(glm::vec3 cameraPosition) {
    int loadNr = 0;
    int startZ = (int)(cameraPosition.z / Chunk::CHUNK_DEPTH) - CHUNK_RADIUS;
    int endZ = (int)(cameraPosition.z / Chunk::CHUNK_DEPTH) + CHUNK_RADIUS;

    int endY = 256 / Chunk::CHUNK_HEIGHT;

    int startX = (int)(cameraPosition.x / Chunk::CHUNK_WIDTH) - CHUNK_RADIUS;
    int endX = (int)(cameraPosition.x / Chunk::CHUNK_WIDTH) + CHUNK_RADIUS;

    for (int z = startZ; z <= endZ; z++) {
        for (int x = startX; x <= endX; x++) {
            for (int y = 0; y < endY; y++) {
                glm::ivec3 pos(x, y, z);
                if (m_LoadedChunks.find(pos) == m_LoadedChunks.end()) {
                    m_LoadList.push(std::async(std::launch::async, Load, pos));
                    // m_LoadList.push_back(Load(pos));
                    m_LoadedChunks.insert(pos);
                    loadNr++;
                    if (loadNr > 8)
                        return;
                }
            }
        }
    }
}

void ChunkManager::CheckChunks() {
}

void ChunkManager::UnloadChunks(glm::vec3 cameraPosition) {
    for (int i = 0; i < Chunks.size(); ++i) {
        glm::ivec2 camChunkCoords(cameraPosition.x / Chunk::CHUNK_WIDTH, cameraPosition.z / Chunk::CHUNK_DEPTH);
        glm::ivec2 chunkSpace = {Chunks[i]->GetPosition().x - camChunkCoords.x, Chunks[i]->GetPosition().z - camChunkCoords.y};

        if (abs(chunkSpace.x) > CHUNK_RADIUS || abs(chunkSpace.y) > CHUNK_RADIUS) {
            if (Chunks[i]->IsLoaded()) {
                m_LoadedChunks.erase(Chunks[i]->GetPosition());
                Chunks.erase(std::remove(Chunks.begin(), Chunks.end(), Chunks[i]), Chunks.end());
            }
        }
    }
}

void ChunkManager::SynchronizeChunks() {
    int nrOfLoad = 0;
    while (m_LoadList.size() != 0 || nrOfLoad >= CHUNKS_PER_FRAME) {
        auto status = m_LoadList.front().wait_for(std::chrono::microseconds(0));
        if (status == std::future_status::ready) {
            Chunks.push_back(m_LoadList.front().get());
            Chunks.back()->UploadToGPU();
            m_LoadList.pop();
        } else {
            break;
        }
    }
}

// void ChunkManager::SynchronizeChunks() {
//     for (int i = 0; i < m_LoadList.size(); i++) {
//         m_LoadList.back()->UploadToGPU();
//         Chunks.push_back(m_LoadList.back());
//         m_LoadList.pop_back();
//     }
// }

ts::Ref<Chunk> ChunkManager::Load(glm::ivec3 pos) {
    ts::Ref<Chunk> chunk(new Chunk());
    chunk->Init(pos);
    chunk->Generate();
    chunk->CreateMesh();
    return chunk;
}

// void ChunkManager::Rebuild() {
// }
