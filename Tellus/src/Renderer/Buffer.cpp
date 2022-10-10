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

VertexBuffer::VertexBuffer(unsigned int* vertices, unsigned int byteSize, const BufferLayout& layout) {
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, byteSize, vertices, GL_STATIC_DRAW);
    m_VertexCount = byteSize / (layout.GetStride() * sizeof(unsigned int));
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
        if (e.Type == GL_UNSIGNED_INT)
            glVertexAttribIPointer(index, e.Count, e.Type, layout.GetStride() * sizeof(unsigned int), (const void*)((e.Offset * sizeof(unsigned int))));
        else
            glVertexAttribPointer(index, e.Count, e.Type, GL_FALSE, layout.GetStride() * sizeof(float), (const void*)((e.Offset * sizeof(e.Type))));
        index++;
    }
    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_RenderID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}

// ---------------------------------------------------------------------------
// ||||||||||||||||||||||||||||FrameBuffer||||||||||||||||||||||||||||||||||||
// ---------------------------------------------------------------------------

static int PrevViewPort[4];

FrameBuffer::FrameBuffer(int width, int height) : m_Width(width), m_Height(height) {
    glCreateFramebuffers(1, &m_RenderID);
}
void FrameBuffer::Bind() const {
    glGetIntegerv(GL_VIEWPORT, PrevViewPort);
    glViewport(0, 0, m_Width, m_Height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
}
void FrameBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(PrevViewPort[0], PrevViewPort[1], PrevViewPort[2], PrevViewPort[3]);
}
bool FrameBuffer::Check() const {
    Bind();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Unbind();
        return false;
    }
    Unbind();
    return true;
}
void FrameBuffer::AddColorRenderBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
void FrameBuffer::AddDepthRenderBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
void FrameBuffer::AddColorTexture() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    m_ColorTextures.push_back(texture);
    // glBindTexture(GL_TEXTURE_2D, 0);
}
void FrameBuffer::AddDepthTexture() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    m_DepthTextures.push_back(texture);
    glBindTexture(GL_TEXTURE_2D, 0);
}

};  // namespace ts