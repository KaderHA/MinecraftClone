#include "Skybox.hpp"

Skybox::Skybox(const char* directoryPath) {
    float skyboxVertices[] = {
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f

    };

    unsigned int skyboxIndices[36];
    for (int i = 0; i < 6; i++) {
        skyboxIndices[i * 6 + 0] = 0 + i * 4;
        skyboxIndices[i * 6 + 1] = 1 + i * 4;
        skyboxIndices[i * 6 + 2] = 3 + i * 4;
        skyboxIndices[i * 6 + 3] = 1 + i * 4;
        skyboxIndices[i * 6 + 4] = 2 + i * 4;
        skyboxIndices[i * 6 + 5] = 3 + i * 4;
    }

    ts::BufferLayout skyboxLayout = {{0x1406, 3}};
    ts::Ref<ts::VertexBuffer> vb(new ts::VertexBuffer(skyboxVertices, sizeof(skyboxVertices), skyboxLayout));
    ts::Ref<ts::IndexBuffer> ib(new ts::IndexBuffer(skyboxIndices, 36));
    m_VertexArray.reset(new ts::VertexArray(vb, ib));

    m_Texture.reset(new ts::TextureCubeMap(directoryPath));
}

Skybox::~Skybox() {}

void Skybox::BindTexture(unsigned int slot) const {
    m_Texture->Bind(slot);
}