#pragma once
#include <Tellus.hpp>

#define SIZE 16

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
};

/**
 * Cube indices:

 * FRONT_BOTTOM_LEFT 0
 * FRONT_BOTTOM_RIGHT 1
 * FRONT_TOP_RIGHT 2
 * FRONT_TOP_LEFT 3
 * BACK_BOTTOM_LEFT 4
 * BACK_BOTTOM_RIGHT 5
 * BACK_TOP_RIGHT 6
 * BACK_TOP_LEFT 7
 **/

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