#pragma once
#include "Core.hpp"

namespace ts {

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
    Texture2D(const std::string& path);
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

};  // namespace ts