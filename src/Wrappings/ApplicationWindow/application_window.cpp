#include "application_window.h"

#include "../../UnSorted/Logger/logger.h"

void ApplicationWindow::CheckWindow() {
    if (_window == NULL) {
        Logger::Fatal("loading", "Failed to create GLFW Window: " + _name);
        throw std::exception();
    }
    Logger::Info("loading", "GLFW Window created: " + _name);
}
void ApplicationWindow::LoadGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Fatal("loading", "Failed to initialize GLAD");
        throw std::exception();
    }
    Logger::Info("loading", "GLAD Initialized");
}

GLFWmonitor* ApplicationWindow::GetMonitor(int id) {
    int monitors_amount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitors_amount);
    return monitors[id];
}
void ApplicationWindow::DefaultFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

ApplicationWindow::ApplicationWindow() {
}

void ApplicationWindow::Init(std::string name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _window = glfwCreateWindow(1080, 720, name.c_str(), NULL, NULL);
    _screen_size_state = ScreenSizeState::windowed;
    CheckWindow();
    glfwSetFramebufferSizeCallback(_window, DefaultFrameBufferSizeCallback);
    glfwMakeContextCurrent(_window);
    LoadGLAD();
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(0);
    SetFullScreen(0);
    Logger::Info("loading", "Application Window initialized: " + _name);
}
void ApplicationWindow::SetFullScreen(int id) {
    GLFWmonitor* monitor = GetMonitor(id);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    _screen_size_state = ScreenSizeState::fullscreen;
}
void ApplicationWindow::SetWindowed() {
    glfwSetWindowMonitor(_window, nullptr, 100, 100, 1080, 720, 200);
    _screen_size_state = ScreenSizeState::windowed;
}

void ApplicationWindow::StartLoop() {
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        _process_input();
        _update_world_func();
        _draw_func();
        glfwSwapBuffers(_window);
    }
}

void ApplicationWindow::SetDrawFunc(void (*draw_func)()) {
    _draw_func = draw_func;
}
void ApplicationWindow::SetUpdateWorldFunc(void (*update_world_func)()) {
    _update_world_func = update_world_func;
}
void ApplicationWindow::SetProcessInputFunc(void (*process_input)()) {
    _process_input = process_input;
}
void ApplicationWindow::SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height)) {
    glfwSetFramebufferSizeCallback(_window, frame_buffer_size_callback);
}

ScreenSizeState ApplicationWindow::GetScreenSizeState() const {
    return _screen_size_state;
}
GLFWwindow* ApplicationWindow::GetWindow() {
    return _window;
}