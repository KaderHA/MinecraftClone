#pragma once
#include "Core.hpp"

namespace ts {

// ---------------------------------------------------------------------------
// ||||||||||||||||||||||||||||BufferLayout|||||||||||||||||||||||||||||||||||
// ---------------------------------------------------------------------------

class BufferLayout {
    struct Element {
        unsigned int Type;
        int Count;
        int Offset;

        Element() : Type(0), Count(0), Offset(0) {}
        Element(unsigned int type, int count, int offset = 0) : Type(type), Count(count), Offset(offset) {}
    };

   public:
    BufferLayout() : m_Elements(0), m_Stride(0) {}
    BufferLayout(const std::initializer_list<Element>& elements) : m_Elements(elements), m_Stride(0) { StrideAndOffset(); }

    std::vector<BufferLayout::Element>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferLayout::Element>::iterator end() { return m_Elements.end(); }

    inline int GetStride() const { return m_Stride; }
    const std::vector<BufferLayout::Element>& GetElements() const { return m_Elements; }

   private:
    void StrideAndOffset() {
        for (auto& element : m_Elements) {
            element.Offset = m_Stride;
            m_Stride += element.Count;
        }
    }

    std::vector<Element> m_Elements;
    int m_Stride;
};

// ---------------------------------------------------------------------------
// ||||||||||||||||||||||||||||VertexBuffer|||||||||||||||||||||||||||||||||||
// ---------------------------------------------------------------------------

class VertexBuffer {
   public:
    VertexBuffer(float* vertices, unsigned int byteSize);
    VertexBuffer(float* vertices, unsigned int byteSize, const BufferLayout& layout);
    ~VertexBuffer();
    void Bind() const;
    void Unbind() const;

    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
    const BufferLayout& GetLayout() const { return m_Layout; }
    const unsigned int GetVertexCount() const { return m_VertexCount; }

    static VertexBuffer* Create(float* vertices, unsigned int byteSize);

   private:
    BufferLayout m_Layout;
    unsigned int m_RenderID, m_VertexCount;
};

// ---------------------------------------------------------------------------
// ||||||||||||||||||||||||||||IndexBuffer||||||||||||||||||||||||||||||||||||
// ---------------------------------------------------------------------------

class IndexBuffer {
   public:
    IndexBuffer(unsigned int* indicies, unsigned int count);
    ~IndexBuffer();
    void Bind() const;
    void Unbind() const;

    unsigned int GetCount() const { return m_Count; }

    static IndexBuffer* Create(unsigned int* indicies, unsigned int count);

   private:
    unsigned int m_RenderID, m_Count;
};

class VertexArray {
   public:
    VertexArray();
    VertexArray(const Ref<VertexBuffer>& vb);
    VertexArray(const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib);
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    bool HasIndexBuffer() const { return m_IndexBuffer != nullptr; }
    int GetCount() const { return m_IndexBuffer->GetCount(); }
    int GetVertexCount() const { return m_VertexBuffers[0]->GetVertexCount(); }

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

   private:
    unsigned int m_RenderID;
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};

};  // namespace ts
