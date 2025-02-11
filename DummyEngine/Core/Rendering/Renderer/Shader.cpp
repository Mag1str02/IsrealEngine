#include "Shader.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLShader.h"

namespace DummyEngine {

    Ref<Shader> Shader::Create(const std::vector<ShaderPart>& initializers) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLShader>(initializers);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create Shader on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create Shader without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }

}  // namespace DummyEngine