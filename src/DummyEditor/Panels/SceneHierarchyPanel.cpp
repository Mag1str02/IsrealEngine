#include "Panels/SceneHierarchyPanel.h"

namespace DE
{
    void SceneHierarchyPanel::SetActiveScene(Ref<Scene> scene) { m_Scene = scene; }
    void SceneHierarchyPanel::View()
    {
        DE_PROFILE_SCOPE("SceneHierarchyPanel View");

        if (m_Scene)
        {
            std::vector<Entity> entities = m_Scene->GetAllEntities();
            for (auto& entity : entities)
            {
                bool selected = false;
                if (m_SelectedEntity.Valid())
                {
                    selected = m_SelectedEntity.GetComponent<IdComponent>() == entity.GetComponent<IdComponent>();
                }
                if (ImGui::Selectable(entity.GetComponent<TagComponent>().tag.c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    if (ImGui::IsMouseDoubleClicked(0))
                    {
                        m_SelectedEntity = entity;
                    }
                }
            }
        }
    }

    Entity SceneHierarchyPanel::GetActiveEntity() { return m_SelectedEntity; }
}  // namespace DE