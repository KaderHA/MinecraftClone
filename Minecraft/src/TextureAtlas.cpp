#include "TextureAtlas.hpp"
#include "Block.hpp"

TextureAtlas::TextureAtlas(const ts::Ref<ts::Texture>& texture, unsigned int row_count, unsigned int column_count) {
    m_TextureCoords = new QuadTextureCoords[((texture->GetWidth() / row_count) * (texture->GetHeight() / column_count))];
    float* uvArray = new float[((texture->GetWidth() / row_count) * (texture->GetHeight() / column_count)) * 8];
    const float spacing = (float)row_count / (float)texture->GetWidth();

    for (int y = column_count, index = 0; y > 0; y--) {
        for (int x = 0; x < row_count; x++, index++) {
            // uvArray[index + 0] = (float)x * spacing;
            // uvArray[index + 1] = (float)y * spacing;

            // uvArray[index + 2] = (float)x * spacing;
            // uvArray[index + 3] = (((float)y * spacing) - spacing);

            // uvArray[index + 4] = ((float)x * spacing) + spacing;
            // uvArray[index + 5] = (((float)y * spacing) - spacing);

            // uvArray[index + 6] = ((float)x * spacing) + spacing;
            // uvArray[index + 7] = (float)y * spacing;
            m_TextureCoords[index].BottomLeft = {(float)x * spacing, (((float)y * spacing) - spacing)};
            m_TextureCoords[index].BottomRight = {((float)x * spacing) + spacing, (((float)y * spacing) - spacing)};
            m_TextureCoords[index].TopRight = {((float)x * spacing) + spacing, (float)y * spacing};
            m_TextureCoords[index].TopLeft = {(float)x * spacing, (float)y * spacing};
        }
    }

    m_UVBuffer.reset(new ts::TextureBuffer(uvArray, (((texture->GetWidth() / row_count) * (texture->GetHeight() / column_count)) * 8) * sizeof(float), ts::TextureDataType::R32F));
}
TextureAtlas::~TextureAtlas() {
    delete[] m_TextureCoords;
}

TexCoords TextureAtlas::GetTextureCoords(BlockType type) const {
    TexCoords texCoords;
    switch (type) {
        case BlockType::Dirt:
            texCoords.top = m_TextureCoords[2];
            texCoords.bottom = m_TextureCoords[2];
            texCoords.side = m_TextureCoords[2];
            return texCoords;
            break;
        case BlockType::Grass:
            texCoords.top = m_TextureCoords[0];
            texCoords.bottom = m_TextureCoords[2];
            texCoords.side = m_TextureCoords[3];
            return texCoords;
            break;

        case BlockType::Stone:
            texCoords.top = m_TextureCoords[1];
            texCoords.bottom = m_TextureCoords[1];
            texCoords.side = m_TextureCoords[1];
            return texCoords;
            break;
        case BlockType::Bedrock:
            texCoords.top = m_TextureCoords[17];
            texCoords.bottom = m_TextureCoords[17];
            texCoords.side = m_TextureCoords[17];
            return texCoords;
            break;
        default:
            texCoords.top = m_TextureCoords[1];
            texCoords.bottom = m_TextureCoords[1];
            texCoords.side = m_TextureCoords[1];
            break;
    }
}