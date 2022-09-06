#include "Panels/ViewportPanel.h"

namespace DE
{
    ViewportPanel::ViewportPanel() : m_ViewportSize(ImVec2(1, 1)) {}

    void ViewportPanel::OnImGuiRender()
    {
        DE_PROFILE_SCOPE("ViewportPanel OnImGuiRender");

        static bool open = true;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        ImGui::PopStyleVar();
        m_ViewportSize = ImGui::GetContentRegionAvail();
        ImGui::Image(reinterpret_cast<void*>(m_FrameBuffer->GetColorAttachment(0)->RendererId()), m_ViewportSize, {0, 1}, {1, 0});
        ImGui::End();
    }
    void ViewportPanel::SetFrameBuffer(Ref<FrameBuffer> buffer) { m_FrameBuffer = buffer; }
}  // namespace DE