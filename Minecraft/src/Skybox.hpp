#pragma once
#include <Tellus.hpp>

class Skybox {
   public:
    Skybox(const char* directoryPath);
    ~Skybox();

    void BindTexture(unsigned int slot) const;
    const ts::Ref<ts::VertexArray>& GetVAO() const { return m_VertexArray; }

   private:
    ts::Ref<ts::VertexArray> m_VertexArray;
    ts::Ref<ts::TextureCubeMap> m_Texture;
};

// clang-format on