#pragma once
#include <Tellus.hpp>

enum class BlockType : unsigned int;

struct QuadTextureCoords {
    glm::vec2 BottomLeft;
    glm::vec2 BottomRight;
    glm::vec2 TopRight;
    glm::vec2 TopLeft;
};

struct TexCoords {
    QuadTextureCoords top;
    QuadTextureCoords bottom;
    QuadTextureCoords side;
};

class TextureAtlas {
   public:
    TextureAtlas(const ts::Ref<ts::Texture>& texture, unsigned int row_count, unsigned int column_count);
    ~TextureAtlas();

    TexCoords GetTextureCoords(BlockType type) const;

   private:
    void GenerateTexCoords();

   private:
    ts::Ref<ts::TextureBuffer> m_UVBuffer;
    QuadTextureCoords* m_TextureCoords;
};
