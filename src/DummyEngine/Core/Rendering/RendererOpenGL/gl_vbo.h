#pragma once

#include <GLAD/glad.h>

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Core/Rendering/Renderer/vertex_buffer.h"

namespace DE
{

    class GLVertexBuffer : public VertexBuffer
    {

    public:
        GLVertexBuffer() = delete;
        GLVertexBuffer(const GLVertexBuffer& other) = delete;
        GLVertexBuffer(GLVertexBuffer&& other) = delete;
        GLVertexBuffer& operator=(const GLVertexBuffer& other) = delete;
        GLVertexBuffer& operator=(GLVertexBuffer&& other) = delete;

        GLVertexBuffer(uint32_t size, BufferUsage usage = BufferUsage::Static);
        GLVertexBuffer(const void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);

        virtual ~GLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual const BufferLayout& GetLayout() const override;
        virtual void SetLayout(const BufferLayout& layout) override;

        virtual void SetData(const void* data, uint32_t size) override;

    private:
        static GLenum BufferUsafeToGLBufferUsage(BufferUsage usage);

        BufferLayout _layout;
        GLuint _buffer_id;
    };

    class GLIndexBuffer : public IndexBuffer
    {
    public:
        GLIndexBuffer() = delete;
        GLIndexBuffer(const GLIndexBuffer& other) = delete;
        GLIndexBuffer(GLIndexBuffer&& other) = delete;
        GLIndexBuffer& operator=(const GLIndexBuffer& other) = delete;
        GLIndexBuffer& operator=(GLIndexBuffer&& other) = delete;

        GLIndexBuffer(const uint32_t* indices, uint32_t count);
        virtual ~GLIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual uint32_t IndicesAmount() const override;

    private:
        GLuint _buffer_id;
        uint32_t _indices_amount;
    };
}  // namespace DE