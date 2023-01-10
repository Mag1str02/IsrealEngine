#include "DummyEngine/Core/Rendering/Renderer/UniformBuffer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGl/GLUniformBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE
{
    Ref<UniformBuffer> UniformBuffer::Create(const BufferLayout& layout, uint32_t size)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLUniformBuffer>(layout, size);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create UniformBuffer on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create UniformBuffer without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }

        return nullptr;
    }
}  // namespace DE