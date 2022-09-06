#pragma once

#include "Addition/Base.h"

namespace DE
{
    class ImGuiPanel
    {
    public:
        ImGuiPanel(const std::string& name) : m_Name(name) {}
        virtual ~ImGuiPanel(){};
        virtual void View() = 0;

        void OnImGuiRender();
        void SetController(bool& enabled) { m_Enabled = &enabled; }

        bool* GetController() { return m_Enabled; }
        const std::string& GetName() const { return m_Name; }

    private:
        bool* m_Enabled = nullptr;
        std::string m_Name;
    };
}  // namespace DE
