#pragma once
#include <Tellus.hpp>
#include "Chunk.hpp"

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const glm::ivec3& position) const {
        return (position.x * 88339) ^ (position.z * 91967) ^ (position.z * 126323);
    }
};

class ChunkManager {
   public:
    static void Update(glm::vec3 cameraPosition);

   private:
    static void LoadChunks(glm::vec3 cameraPosition);
    static void UnloadChunks(glm::vec3 cameraPosition);
    static void SynchronizeChunks();
    static void RebuildChunks();

   public:
    static std::vector<ts::Scope<Chunk>> Chunks;

   private:
    static ts::Scope<Chunk> Load(glm::ivec3 pos);
    // static void Rebuild();

   private:
    // static std::unordered_map<glm::ivec3, ts::Ref<Chunk>, ChunkPositionHash> m_Chunks;
    static std::unordered_set<glm::ivec3, ChunkPositionHash> m_LoadedChunks;

    // Multithreaded
    static std::queue<std::future<ts::Scope<Chunk>>> m_LoadList;
    // SingleThread
    // static std::vector<ts::Ref<Chunk>> m_LoadList;
};