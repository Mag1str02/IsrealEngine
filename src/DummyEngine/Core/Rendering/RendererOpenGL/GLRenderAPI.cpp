#include <GLAD/glad.h>

#include "Core/Rendering/RendererOpenGL/GLRenderAPI.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{

    void GLRenderAPI::SetDefaultState()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        // glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);
        // glDepthMask(GL_TRUE);
        // glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void GLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void GLRenderAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    void GLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertex_array)
    {
        vertex_array->Bind();
        glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, nullptr);
    }
    void GLRenderAPI::DrawInstanced(const Ref<VertexArray>& vertex_array, uint32_t instance_count)
    {
        vertex_array->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, vertex_array->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, nullptr, instance_count);
    }

    void GLRenderAPI::SetClearColor(const Vec4& color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }
    void GLRenderAPI::SetLineWidth(float width)
    {
        glLineWidth(width);
    }
    void GLRenderAPI::Enable(RenderSetting setting)
    {
        switch (setting)
        {
            case RenderSetting::DepthTest: glEnable(GL_DEPTH_TEST);
            case RenderSetting::MultiSampling: glEnable(GL_MULTISAMPLE);
            case RenderSetting::FaceCulling: glEnable(GL_CULL_FACE);
        }
    }
    void GLRenderAPI::Disable(RenderSetting setting)
    {
        switch (setting)
        {
            case RenderSetting::DepthTest: glDisable(GL_DEPTH_TEST);
            case RenderSetting::MultiSampling: glDisable(GL_MULTISAMPLE);
            case RenderSetting::FaceCulling: glDisable(GL_CULL_FACE);
        }
    }

    API GLRenderAPI::GetAPI()
    {
        return API::OpenGL;
    }
}  // namespace DE