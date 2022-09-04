#include "Chunk.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
};

Chunk::Chunk() {}
Chunk::~Chunk() {}

void Chunk::Generate(int localChunkX, int localChunkY, int localChunkZ) {}
void Chunk::CreateMesh() {
    Vertex* vertices = new Vertex[CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * 24];
    int index = 0;
    for (int z = 0; z < CHUNK_DEPTH; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                // Front
                vertices[index++] = {{(float)x, (float)y, (float)z + 1.0f}, {0.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, {1.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, {1.0f, 1.0f}};
                vertices[index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, {0.0f, 1.0f}};

                // Back
                vertices[index++] = {{(float)x, (float)y, (float)z}, {0.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y, (float)z}, {1.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, {1.0f, 1.0f}};
                vertices[index++] = {{(float)x, (float)y + 1.0f, (float)z}, {0.0f, 1.0f}};

                // Right
                vertices[index++] = {{(float)x + 1.0f, (float)y, (float)z}, {0.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, {1.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, {1.0f, 1.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, {0.0f, 1.0f}};

                // Left
                vertices[index++] = {{(float)x, (float)y, (float)z}, {0.0f, 0.0f}};
                vertices[index++] = {{(float)x, (float)y, (float)z + 1.0f}, {1.0f, 0.0f}};
                vertices[index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, {1.0f, 1.0f}};
                vertices[index++] = {{(float)x, (float)y + 1.0f, (float)z}, {0.0f, 1.0f}};

                // Top
                vertices[index++] = {{(float)x, (float)y + 1.0f, (float)z}, {0.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z}, {1.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f}, {1.0f, 1.0f}};
                vertices[index++] = {{(float)x, (float)y + 1.0f, (float)z + 1.0f}, {0.0f, 1.0f}};

                // Bottom

                vertices[index++] = {{(float)x, (float)y, (float)z}, {0.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y, (float)z}, {1.0f, 0.0f}};
                vertices[index++] = {{(float)x + 1.0f, (float)y, (float)z + 1.0f}, {1.0f, 1.0f}};
                vertices[index++] = {{(float)x, (float)y, (float)z + 1.0f}, {0.0f, 1.0f}};
            }
        }
    }

    float* float_vertices = new float[CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * 24 * 5];

    for (int i = 0; i < CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * 24; i++) {
        float_vertices[(i * 5) + 0] = vertices[i].Position.x;
        float_vertices[(i * 5) + 1] = vertices[i].Position.y;
        float_vertices[(i * 5) + 2] = vertices[i].Position.z;
        float_vertices[(i * 5) + 3] = vertices[i].TexCoord.x;
        float_vertices[(i * 5) + 4] = vertices[i].TexCoord.y;
    }

    ts::BufferLayout layout = {{0x1406, 3}, {0x1406, 2}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(float_vertices, (CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * 24 * 5 * sizeof(float)), layout));
    unsigned int* indices = CreateIndices();
    ts::Ref<ts::IndexBuffer> ib(new ts::IndexBuffer(indices, (CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * 36)));
    m_VertexArray.reset(new ts::VertexArray(vb, ib));

    delete[] vertices;
    delete[] float_vertices;
    delete[] indices;
}

unsigned int* Chunk::CreateIndices() {
    unsigned int* indices = new unsigned int[CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * 36];
    for (int i = 0, index = 0; i < CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH * 36; i += 36, index += 24) {
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