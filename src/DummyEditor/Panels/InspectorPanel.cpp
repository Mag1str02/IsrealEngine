#include "Panels/InspectorPanel.h"

namespace DE
{
    void InspectorPanel::View()
    {
        DE_PROFILE_SCOPE("InspectorPanel View");

        float sensitivity = 0.1;

        if (m_Entity.Valid())
        {
            if (m_Entity.HasComponent<TagComponent>())
            {
                auto& component = m_Entity.GetComponent<TagComponent>();
                if (ImGui::CollapsingHeader("Tag"))
                {
                    ImGui::InputText("", &component.tag);
                }
            }
            if (m_Entity.HasComponent<IdComponent>())
            {
                auto& component = m_Entity.GetComponent<IdComponent>();
                if (ImGui::CollapsingHeader("UUID"))
                {
                    ImGui::Text(component.Hex().c_str());
                }
            }
            if (m_Entity.HasComponent<TransformComponent>())
            {
                if (ImGui::CollapsingHeader("Transformation"))
                {
                    auto& transform = m_Entity.GetComponent<TransformComponent>();

                    ImGui::Text("Translation");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3("##Translation", &(transform.translation.x), sensitivity, 0, 0);

                    ImGui::Text("Scale");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3("##Scale", &(transform.scale.x), sensitivity, 0, 0);

                    ImGui::Text("Rotation");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat3("##Rotation", &(transform.rotation.x), sensitivity, 0, 0);
                }
            }
            if (m_Entity.HasComponent<RenderMeshComponent>())
            {
                if (ImGui::CollapsingHeader("Render Mesh"))
                {
                    auto& render_mesh = m_Entity.GetComponent<RenderMeshComponent>();
                    ImGui::Text(("Mesh UUID: " + render_mesh.id.Hex()).c_str());
                }
            }
            if (m_Entity.HasComponent<FPSCamera>())
            {
                if (ImGui::CollapsingHeader("FPSCamera"))
                {
                    auto& camera = m_Entity.GetComponent<FPSCamera>();

                    ImGui::Text("Near Plane");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat("##Near Plane", &(camera.m_NearPlane), sensitivity, 0, 0);

                    ImGui::Text("Far Plane");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat("##Far Plane", &(camera.m_FarPlane), sensitivity, 0, 0);

                    ImGui::Text("FOV");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::DragFloat("##FOV", &(camera.m_FOV), sensitivity, 0, 360);
                }
            }
            if (m_Entity.HasComponent<LightSource>())
            {
                if (ImGui::CollapsingHeader("Light Soruce"))
                {
                    auto& source = m_Entity.GetComponent<LightSource>();
                    ImGui::Text("Ambient");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit3("##Ambient", &(source.ambient.x));

                    ImGui::Text("Diffuse");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit3("##Diffuse", &(source.diffuse.x));

                    ImGui::Text("Specular");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-1);
                    ImGui::ColorEdit3("##Specular", &(source.specular.x));

                    ImGui::Text("CLQ");
                    ImGui::SameLine(100);
                    ImGui::SetNextItemWidth(-24);
                    ImGui::DragFloat3("##CLQ", &(source.clq.x), 0, 1);
                }
            }
        }
    }
    void InspectorPanel::SetActiveEntity(Entity entity) { m_Entity = entity; }

}  // namespace DE