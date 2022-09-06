#pragma once
#include "Core.hpp"

namespace ts {

enum class TextureDataType {
    R8 = 0x8229,
    R16 = 0x822A,
    RG8 = 0x822B,
    RG16 = 0x822C,
    R16F = 0x822D,
    R32F = 0x822E,
    RG16F = 0x822F,
    RG32F = 0x8230,
    R8I = 0x8231,
    R8UI = 0x8232,
    R16I = 0x8233,
    R16UI = 0x8234,
    R32I = 0x8235,
    R32UI = 0x8236,
    RG8I = 0x8237,
    RG8UI = 0x8238,
    RG16I = 0x8239,
    RG16UI = 0x823A,
    RG32I = 0x823B,
    RG32UI = 0x823C,
    RGBA32F = 0x8814,
    RGB32F = 0x8815,
    RGBA16F = 0x881A,
    RGB16F = 0x881B,
    RGBA32UI = 0x8D70,
    RGB32UI = 0x8D71,
    RGBA16UI = 0x8D76,
    RGB16UI = 0x8D77,
    RGBA8UI = 0x8D7C,
    RGB8UI = 0x8D7D,
    RGBA32I = 0x8D82,
    RGB32I = 0x8D83,
    RGBA16I = 0x8D88,
    RGB16I = 0x8D89,
    RGBA8I = 0x8D8E,
    RGB8I = 0x8D8F
};

class Texture {
   public:
    virtual ~Texture() = default;
    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual void Bind(unsigned int slot = 0) const = 0;
    virtual void Unbind(unsigned int slot) const = 0;
};

class Texture2D : public Texture {
   public:
    Texture2D(const std::string& filepath);
    virtual ~Texture2D();

    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }

    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind(unsigned int slot) const override;

    // For Multiple rendering backends
    // static Ref<Texture2D> Create(const std::string& path);

   private:
    unsigned int m_ID;
    unsigned int m_Width, m_Height;
};

class TextureCubeMap : public Texture {
   public:
    TextureCubeMap(const std::string& directoryPath);
    virtual ~TextureCubeMap();

    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }

    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind(unsigned int slot) const override;

    // For Multiple rendering backends
    // static Ref<Texture2D> Create(const std::string& path);

   private:
    unsigned int m_ID;
    unsigned int m_Width, m_Height;
};

class TextureBuffer : public Texture {
   public:
    TextureBuffer(float* data, unsigned int byteSize, TextureDataType type);
    virtual ~TextureBuffer();

    virtual unsigned int GetWidth() const override { return m_Width; }
    virtual unsigned int GetHeight() const override { return m_Height; }

    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind(unsigned int slot) const override;

   private:
    unsigned int m_ID;
    unsigned int m_Width, m_Height;
};

};  // namespace ts