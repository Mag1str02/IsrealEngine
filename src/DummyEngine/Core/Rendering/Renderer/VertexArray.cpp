#include "Core/Rendering/Renderer/VertexArray.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/Rendering/RendererOpenGL/GLVertexArray.h"

namespace DE
{

    Ref<VertexArray> VertexArray::Copy(const VertexArray& other) {
        Ref<VertexArray> result = VertexArray::Create();
        for(const auto& v : other.GetVertexBuffers()){
            result->AddVertexBuffer(v);
        }
        result->SetIndexBuffer(other.GetIndexBuffer());
        return result;
    }
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLVertexArray>();
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create VertexArray on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create VertexArray without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }
}  // namespace DE