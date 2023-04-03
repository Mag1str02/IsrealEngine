#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE
{
    class ProfilerPanel : public ImGuiPanel
    {
    public:
        ProfilerPanel() : ImGuiPanel("Profiler"){}
        virtual void View() override;

    private:
        void RenderTimeLapse(const std::vector<TimeLapse>& time_lapses, U32 index);
    };
}  // namespace DE