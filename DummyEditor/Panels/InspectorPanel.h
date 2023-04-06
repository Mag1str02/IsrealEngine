#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    class InspectorPanel : public ImGuiPanel {
    public:
        InspectorPanel() : ImGuiPanel("InspectorPanel") {}
        virtual void OnImGui() override;

        void SetActiveEntity(Entity entity);
        void SetScene(WeakRef<Scene> scene);

    private:
        template <typename Component> void DrawComponentWidget(Entity entity);

        Entity         m_Entity;
        WeakRef<Scene> m_Scene;

        float m_SliderSensitivity = 0.1;
    };
}  // namespace DE