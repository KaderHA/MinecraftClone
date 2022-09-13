#include "ChunkManager.hpp"
#include "Chunk.hpp"

#define CHUNK_RADIUS 4

std::vector<ts::Ref<Chunk>> ChunkManager::Chunks;
std::unordered_set<glm::ivec3, ChunkPositionHash> ChunkManager::m_LoadedChunks;
std::vector<std::future<ts::Ref<Chunk>>> ChunkManager::m_LoadList;

void ChunkManager::Update(glm::vec3 cameraPosition) {
    LoadChunks(cameraPosition);
    UnloadChunks(cameraPosition);
    SynchronizeChunks();
}

void ChunkManager::LoadChunks(glm::vec3 cameraPosition) {
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
                    m_LoadList.push_back(std::async(std::launch::async, Load, pos));
                    m_LoadedChunks.insert(pos);
                    return;
                }
            }
        }
    }
}

void ChunkManager::UnloadChunks(glm::vec3 cameraPosition) {
    for (int i = 0; i < Chunks.size(); ++i) {
        glm::ivec2 camChunkCoords(cameraPosition.x / Chunk::CHUNK_WIDTH, cameraPosition.z / Chunk::CHUNK_DEPTH);
        glm::ivec2 chunkSpace = {Chunks[i]->GetPosition().x - camChunkCoords.x, Chunks[i]->GetPosition().z - camChunkCoords.y};

        if (abs(chunkSpace.x) > CHUNK_RADIUS || abs(chunkSpace.y) > CHUNK_RADIUS) {
            if (Chunks[i]->IsLoaded()) {
                Chunks.erase(std::remove(Chunks.begin(), Chunks.end(), Chunks[i]), Chunks.end());
                m_LoadedChunks.erase(Chunks[i]->GetPosition());
            }
        }
    }
}

void ChunkManager::SynchronizeChunks() {
    for (auto itr = m_LoadList.begin(); itr != m_LoadList.end();) {
        auto status = itr->wait_for(std::chrono::microseconds(0));
        if (status == std::future_status::ready) {
            Chunks.push_back(itr->get());
            Chunks.back()->UploadToGPU();
            itr = m_LoadList.erase(itr);
        } else
            itr++;
    }
}

ts::Ref<Chunk> ChunkManager::Load(glm::ivec3 pos) {
    ts::Ref<Chunk> chunk(new Chunk());
    chunk->Init(pos);
    chunk->Generate();
    chunk->CreateMesh();
    return chunk;
}

// void ChunkManager::Upload() {
// }

// void ChunkManager::Rebuild() {
// }
