#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

const Vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
const Vec3 COLOR_WHITE(1.0f, 1.0f, 1.0f);
const Vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const Vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const Vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const Vec3 COLOR_YELLOW(1.0f, 1.0f, 0.0f);

class MovingSystem : public System
{
public:
    MovingSystem() {}
    virtual std::string GetName() const override
    {
        return "MovingSystem";
    }

    void Update(double dt) override
    {
        auto& positions = GetComponentArray<TransformComponent>();
        auto& meshes = GetComponentArray<RenderMeshComponent>();
        auto& light_sources = GetComponentArray<LightSource>();

        for (auto [id, mesh] : meshes)
        {
            mesh.mesh_instance->at<Mat4>(0) = positions[id].GetTransform();
        }

        for (auto [entity_id, light_source] : light_sources)
        {
            if (positions.HasComponent(entity_id))
            {
                light_source.position = positions[entity_id].translation;
            }
        }
    }
};

class MyLayer : public Layer
{
public:
    MyLayer() : Layer("LoadingScene")
    {
        m_EventDispatcher.AddEventListener<WindowResizeEvent>(
            [this](WindowResizeEvent& event)
            {
                FPSCamera& camera = scene->GetByName("player").GetComponent<FPSCamera>();
                if (event.GetWidth() != 0 && event.GetHeight() != 0)
                {
                    camera.SetAspect(double(event.GetWidth()) / event.GetHeight());
                }
            });
    }

    virtual void OnAttach() override
    {
        Logger::Stage("loading", "Main", "INITIALIZETION");
        Renderer::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        scene = CreateRef<Scene>("NewScene");
        SceneLoader::Load(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
        scene->RegisterSystem<MovingSystem>();

        m_FrameBuffer = FrameBuffer::Create({1920, 1080});
        m_FrameBuffer->AddColorAttachment(TextureFormat::RGBA);
        m_FrameBuffer->SetDepthAttachment(TextureFormat::DepthStencil);

        m_ScreenShader = Shader::Create({{ShaderPartType::Vertex, Config::GetPath(DE_CFG_ASSET_PATH) / "Shaders" / "FrameBuffer" / "FrameBuffer.vs"},
                                         {ShaderPartType::Fragment, Config::GetPath(DE_CFG_ASSET_PATH) / "Shaders" / "FrameBuffer" / "FrameBuffer.fs"}});
    }
    virtual void OnUpdate(float dt) override
    {
        DE_PROFILE_SCOPE("Processing Input", ProcessInput(dt));
        DE_PROFILE_SCOPE("Scene Update", scene->OnUpdate(dt));
        DE_PROFILE_SCOPE("Scene Render", scene->Render());
    }
    virtual void OnDetach() override
    {
        // SceneLoader::Save(scene, Config::GetPath(DE_CFG_ASSET_PATH) / "Scenes" / "Gallery.yml");
    }
    virtual void OnEvent(Event& event) override
    {
        m_EventDispatcher.Dispatch(event);
    }

private:
    void ProcessInput(float dt)
    {
        float sensitivity = 0.07;
        float speed = 15;

        if (Input::KeyReleased(GLFW_KEY_TAB))
        {
            SetMouseLockToggleEvent event;
            BroadcastEvent(event);
        }

        if (Input::KeyReleased(GLFW_KEY_F11))
        {
            if (!windowed)
            {
                SetWindowModeWindowedEvent event;
                BroadcastEvent(event);
            }
            else
            {
                int monitor_id = 0;
                if (Input::KeyDown(GLFW_KEY_1))
                {
                    monitor_id = 1;
                }

                SetWindowModeFullscreenEvent event(monitor_id);
                BroadcastEvent(event);
            }
            windowed = !windowed;
        }

        if (!Input::MouseLocked())
        {
            return;
        }

        FPSCamera& camera = scene->GetByName("player").GetComponent<FPSCamera>();
        camera.RotateY(Input::CursorXOffset() * sensitivity);
        camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

        if (Input::KeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            speed = 100.0f;
        }
        if (Input::KeyDown(GLFW_KEY_ESCAPE))
        {
            WindowCloseEvent event;
            BroadcastEvent(event);
        }
        if (Input::KeyDown(GLFW_KEY_S))
        {
            camera.MoveInLocal(Vec3(0.0f, 0.0f, -1.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_W))
        {
            camera.MoveInLocal(Vec3(0.0f, 0.0f, 1.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_D))
        {
            camera.MoveInLocal(Vec3(1.0f, 0.0f, 0.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_A))
        {
            camera.MoveInLocal(Vec3(-1.0f, 0.0f, 0.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_SPACE))
        {
            camera.MoveInWorld(Vec3(0.0f, 1.0f, 0.0f) * speed * dt);
        }
        if (Input::KeyDown(GLFW_KEY_C))
        {
            camera.MoveInWorld(Vec3(0.0f, -1.0f, 0.0f) * speed * dt);
        }
    }

    Ref<Shader> m_ScreenShader;
    Ref<FrameBuffer> m_FrameBuffer;
    EventDispatcher m_EventDispatcher;
    Ref<Scene> scene;
    bool windowed = true;
};

namespace DE
{
    Application* CreateApplication()
    {
        Application* app = new Application("MyApplication");
        app->PushLayer(new MyLayer());
        return app;
    }
}  // namespace DE
