#pragma once

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Core/Wrappings/window.h"
#include "DummyEngine/Core/Rendering/Renderer/vertex_array.h"

namespace DE
{
    enum class API
    {
        None = 0,
        OpenGL,
        Vulkan
    };
    enum class RenderSetting
    {
        None = 0,
        DepthTest,
        MultiSampling
    };

    class RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;
        virtual void SetDefaultState() = 0;

        virtual bool Load(const Window* window) = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void Clear() = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

        virtual void SetLineWidth(float width) = 0;
        virtual void SetClearColor(const Vec4& color) = 0;
        virtual void Enable(RenderSetting setting) = 0;
        virtual void Disable(RenderSetting setting) = 0;

        virtual API GetAPI() = 0;
    };
}  // namespace DE