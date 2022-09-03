#pragma once
#include <Tellus.hpp>

#define SIZE 16

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
};

static float* CreateChunk() {
    Vertex* vertices = new Vertex[SIZE * SIZE * SIZE * 24];
    int index = 0;
    for (int z = 0; z < SIZE; z++) {
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
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

    float* float_vertices = new float[SIZE * SIZE * SIZE * 24 * 5];

    for (int i = 0; i < SIZE * SIZE * SIZE * 24; i++) {
        float_vertices[(i * 5) + 0] = vertices[i].Position.x;
        float_vertices[(i * 5) + 1] = vertices[i].Position.y;
        float_vertices[(i * 5) + 2] = vertices[i].Position.z;
        float_vertices[(i * 5) + 3] = vertices[i].TexCoord.x;
        float_vertices[(i * 5) + 4] = vertices[i].TexCoord.y;
    }
    return float_vertices;
}

static unsigned int* CreateIndices() {
    unsigned int* indices = new unsigned int[SIZE * SIZE * SIZE * 36];
    for (int i = 0, index = 0; i < SIZE * SIZE * SIZE * 36; i += 36, index += 24) {
        for (int j = 0; j < 6; j++) {
            indices[i + (j * 6) + 0] = 0 + (j * 4) + index;
            indices[i + (j * 6) + 1] = 1 + (j * 4) + index;
            indices[i + (j * 6) + 2] = 3 + (j * 4) + index;
            indices[i + (j * 6) + 3] = 1 + (j * 4) + index;
            indices[i + (j * 6) + 4] = 2 + (j * 4) + index;
            indices[i + (j * 6) + 5] = 3 + (j * 4) + index;
        }
    }

    for (int i = 0; i < 36 * 2;) {
        for (int j = 0; j < 6; j++) {
            std::cout << indices[i + j] << ", ";
        }
        std::cout << '\n';
        i += 6;
    }

    return indices;
}

// clang-format off
// float vertices[] = {
//         // Position       // Texcoord   // Normal
//         // Front
//         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//         1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//         1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//         0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

//         // Back
//         0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//         1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//         1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//         0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

//         // Right
//         1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//         1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//         1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//         1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

//         // Left
//         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//         0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//         0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//         0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

//         // Top
//         0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//         1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
//         1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//         0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

//         // Bottom
//         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//         1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//         1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//         0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
// };

unsigned int indices[] = {
    // Front
    0, 1, 3,
    1, 2, 3,

    // Back
    4, 5, 7,
    5, 6, 7,

    // Right
    8, 9, 11,
    9, 10, 11,

    // Left
    12, 13, 15,
    13, 14, 15,

    // Top
    16, 17, 19,
    17, 18, 19,

    // Bottom
    20, 21, 23,
    21, 22, 23,
    
    // 0 + 24, 1 + 24, 3 + 24,
    // 1+ 24, 2+ 24, 3+ 24,

    // // Back
    // 4+ 24, 5+ 24, 7+ 24,
    // 5+ 24, 6+ 24, 7+ 24,

    // // Right
    // 8+ 24, 9+ 24, 11+ 24,
    // 9+ 24, 10+ 24, 11+ 24,

    // // Left
    // 12+ 24, 13+ 24, 15+ 24,
    // 13+ 24, 14+ 24, 15+ 24,

    // // Top
    // 16+ 24, 17+ 24, 19+ 24,
    // 17+ 24, 18+ 24, 19+ 24,

    // // Bottom
    // 20+ 24, 21+ 24, 23+ 24,
    // 21+ 24, 22+ 24, 23+ 24
};
// clang-format on