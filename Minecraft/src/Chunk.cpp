#include "Chunk.hpp"
#include "TextureAtlas.hpp"

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
    for (int i = 0; i < CHUNK_SIZE; i++) {
        m_Blocks[i].SetBlockType(BlockType::Grass);
    }
    // for (int z = 0; z < CHUNK_DEPTH; z++) {
    //     for (int y = 0; y < CHUNK_HEIGHT; y++) {
    //         for (int x = 0; x < CHUNK_WIDTH; x++) {
    //             int average = (CHUNK_WIDTH + CHUNK_HEIGHT + CHUNK_DEPTH) / 3;
    //             if (sqrt((float)(x - CHUNK_WIDTH / 2) * (x - CHUNK_WIDTH / 2) + (y - CHUNK_HEIGHT / 2) * (y - CHUNK_HEIGHT / 2) + (z - CHUNK_DEPTH / 2) * (z - CHUNK_DEPTH / 2)) <= average / 2) {
    //                 int index = (z * CHUNK_HEIGHT * CHUNK_WIDTH) + (y * CHUNK_WIDTH) + x;
    //                 m_Blocks[index].SetBlockType(BlockType::Grass);
    //             }
    //         }
    //     }
    // }
}
void Chunk::CreateMesh(const ts::Ref<TextureAtlas>& texture) {
    Vertex* vertices = new Vertex[CHUNK_SIZE * 24];
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
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopLeft};

                // Back
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z}, texCoords.side.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.side.TopLeft};

                // Right
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.side.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, texCoords.side.TopLeft};

                // Left
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z}, texCoords.side.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z + 1.0f}, texCoords.side.BottomRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, texCoords.side.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.side.TopLeft};

                // Top
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z}, texCoords.top.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, texCoords.top.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, texCoords.top.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, texCoords.top.TopLeft};

                // Bottom

                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z}, texCoords.bottom.BottomLeft};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z}, texCoords.bottom.BottomRight};
                vertices[block_vertex_index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, texCoords.bottom.TopRight};
                vertices[block_vertex_index++] = {{(float)x, (float)y, (float)z + 1.0f}, texCoords.bottom.TopLeft};
            }
        }
    }

    float* float_vertices = new float[CHUNK_SIZE * 24 * 5];

    for (int i = 0; i < CHUNK_SIZE * 24; i++) {
        float_vertices[(i * 5) + 0] = vertices[i].Position.x;
        float_vertices[(i * 5) + 1] = vertices[i].Position.y;
        float_vertices[(i * 5) + 2] = vertices[i].Position.z;
        float_vertices[(i * 5) + 3] = vertices[i].TexCoord.x;
        float_vertices[(i * 5) + 4] = vertices[i].TexCoord.y;
    }

    ts::BufferLayout layout = {{0x1406, 3}, {0x1406, 2}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(float_vertices, (CHUNK_SIZE * 24 * 5 * sizeof(float)), layout));
    unsigned int* indices = CreateIndices();
    ts::Ref<ts::IndexBuffer> ib(new ts::IndexBuffer(indices, (CHUNK_SIZE * 36)));
    m_VertexArray.reset(new ts::VertexArray(vb, ib));

    delete[] vertices;
    delete[] float_vertices;
    delete[] indices;
}

unsigned int* Chunk::CreateIndices() {
    unsigned int* indices = new unsigned int[CHUNK_SIZE * 36];
    for (int i = 0, index = 0; i < CHUNK_SIZE * 36; i += 36, index += 24) {
        for (int j = 0; j < 6; j++) {
            indices[i + (j * 6) + 0] = 0 + (j * 4) + index;
            indices[i + (j * 6) + 1] = 1 + (j * 4) + index;
            indices[i + (j * 6) + 2] = 3 + (j * 4) + index;
            indices[i + (j * 6) + 3] = 1 + (j * 4) + index;
            indices[i + (j * 6) + 4] = 2 + (j * 4) + index;
            indices[i + (j * 6) + 5] = 3 + (j * 4) + index;
        }
    }

    return indices;
}