#include "Buffer.hpp"

#include "tspch.h"
#include "Core.hpp"

#include <glad/glad.h>

namespace ts {

// ---------------------------------------------------------------------------
// ||||||||||||||||||||||||||||VertexBuffer|||||||||||||||||||||||||||||||||||
// ---------------------------------------------------------------------------
VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int byteSize) {
    return new VertexBuffer(vertices, byteSize);
}

VertexBuffer::VertexBuffer(float* vertices, unsigned int byteSize) {
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, byteSize, vertices, GL_STATIC_DRAW);
}
VertexBuffer::VertexBuffer(float* vertices, unsigned int byteSize, const BufferLayout& layout) {
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, byteSize, vertices, GL_STATIC_DRAW);
    m_VertexCount = byteSize / (layout.GetStride() * sizeof(float));
    m_Layout = layout;
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_RenderID); }

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RenderID); }
void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// ---------------------------------------------------------------------------
// ||||||||||||||||||||||||||||IndexBuffer||||||||||||||||||||||||||||||||||||
// ---------------------------------------------------------------------------
IndexBuffer* IndexBuffer::Create(unsigned int* indicies, unsigned int count) {
    return new IndexBuffer(indicies, count);
}

IndexBuffer::IndexBuffer(unsigned int* indicies, unsigned int count) : m_Count(count) {
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(indicies[0]), indicies, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_RenderID); }

void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID); }
void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

// ---------------------------------------------------------------------------
// ||||||||||||||||||||||||||||VertexArray||||||||||||||||||||||||||||||||||||
// ---------------------------------------------------------------------------

VertexArray::VertexArray() : m_IndexBuffer(nullptr) {
    glCreateVertexArrays(1, &m_RenderID);
}

VertexArray::VertexArray(const Ref<VertexBuffer>& vb) : m_IndexBuffer(nullptr) {
    glCreateVertexArrays(1, &m_RenderID);
    this->AddVertexBuffer(vb);
}

VertexArray::VertexArray(const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib) {
    glCreateVertexArrays(1, &m_RenderID);
    this->AddVertexBuffer(vb);
    this->SetIndexBuffer(ib);
}

VertexArray::~VertexArray() {
    m_VertexBuffers.clear();
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RenderID);
}
void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
    TS_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer does not have a valid layout!");
    glBindVertexArray(m_RenderID);
    vertexBuffer->Bind();
    int index = 0;
    const auto& layout = vertexBuffer->GetLayout();

    for (const auto& e : layout.GetElements()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, e.Count, e.Type, GL_FALSE, layout.GetStride() * sizeof(e.Type), (const void*)((e.Offset * sizeof(e.Type))));
        index++;
    }
    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_RenderID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}

};  // namespace ts