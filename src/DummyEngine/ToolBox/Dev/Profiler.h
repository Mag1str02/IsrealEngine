#pragma once

#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>

namespace DE
{
    struct TimeLapse
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_End;
        std::vector<uint32_t> m_Childs;
        std::string m_Name;

        TimeLapse(const std::string& name) : m_Name(name) {}
    };
    struct ProfilerFrame
    {
        std::vector<TimeLapse> m_TimeLapses;

        ProfilerFrame(uint32_t predicted_lapse_amount);
    };

    class Profiler
    {
    private:
        Profiler();

        void IBeginFrame();

        std::queue<ProfilerFrame> m_Frames;
        std::stack<uint32_t> m_TimeLapseStack;
        uint32_t m_PrevFrameTimeLapseAmount;

        void PushTimeLapse(const std::string& name);
        void PopTimeLapse();

        static Profiler& Get();

    public:
        friend class ProfilerScopeObject;

        static void BeginFrame();
    };

    class ProfilerScopeObject
    {
    public:
        ProfilerScopeObject(const std::string& name);
        ~ProfilerScopeObject();
    };

#if DE_PROFILER_ENABLED
#define DE_PROFILE_SCOPE(name) ProfilerScopeObject profiler_scope_object(name)
#define DE_PROFILER_BEGIN_FRAME() Profiler::BeginFrame()
#else
#define DE_PROFILE_SCOPE(name)
#define DE_PROFILER_BEGIN_FRAME() 
#endif

}  // namespace DE