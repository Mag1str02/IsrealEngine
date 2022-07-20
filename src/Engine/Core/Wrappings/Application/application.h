#pragma once
#include <vector>

#include "../Window/window.h"

namespace DE {
class Application {
public:
    Application(std::string name);
    virtual ~Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    virtual void Start();
    virtual void OnLoad();
    virtual void Update(double dt);

protected:
    Window* _window;

private:
    void SetDefaultGLFWSettings();
    bool InitGLFW();
};
}  // namespace DE