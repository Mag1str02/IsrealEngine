#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/ToolBox/Loaders/SceneLoader.h"
#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/SceneRenderData.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"

namespace DE
{
    Ref<Scene> SceneLoader::m_Scene;

    //*~~~Saving~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    YAML::Node NodeVec2(Vec2 vec)
    {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);

        return res;
    }
    YAML::Node NodeVec3(Vec3 vec)
    {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);
        res.push_back(vec.z);

        return res;
    }
    YAML::Node NodeVec4(Vec4 vec)
    {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);
        res.push_back(vec.z);
        res.push_back(vec.w);

        return res;
    }

    YAML::Node NodeScriptField(ScriptField& field)
    {
        switch (field.GetType())
        {
            case ScriptFieldType::Float: return YAML::Node(field.Get<float>());
            case ScriptFieldType::Double: return YAML::Node(field.Get<double>());
            case ScriptFieldType::Bool: return YAML::Node(field.Get<bool>());
            case ScriptFieldType::String: return YAML::Node(field.Get<std::string>());
            case ScriptFieldType::I32: return YAML::Node(field.Get<int32_t>());
            case ScriptFieldType::I64: return YAML::Node(field.Get<int64_t>());
            case ScriptFieldType::UI32: return YAML::Node(field.Get<uint32_t>());
            case ScriptFieldType::UI64: return YAML::Node(field.Get<uint64_t>());
            case ScriptFieldType::Vec2: return NodeVec2(field.Get<Vec2>());
            case ScriptFieldType::Vec3: return NodeVec3(field.Get<Vec3>());
            case ScriptFieldType::Vec4: return NodeVec4(field.Get<Vec4>());
            default: return YAML::Node();
        }
    }

    template <typename ComponentType> void SceneLoader::SaveComponent(YAML::Node& n_Entity, Entity entity)
    {
        // TODO: Different names for different unknown components...
        DE_ASSERT(false, "Trying to save unknown component.");
    }
    template <> void SceneLoader::SaveComponent<TagComponent>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<TagComponent>())
        {
            n_Entity["Tag"] = entity.Get<TagComponent>().tag;
        }
    }
    template <> void SceneLoader::SaveComponent<IdComponent>(YAML::Node& n_Entity, Entity entity)
    {
        // TODO: Save UUID in hex format.
        if (entity.Has<IdComponent>())
        {
            n_Entity["UUID"] = (uint64_t)entity.Get<IdComponent>();
        }
    }
    template <> void SceneLoader::SaveComponent<TransformComponent>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<TransformComponent>())
        {
            n_Entity["Transformation"]["Translation"]       = NodeVec3(entity.Get<TransformComponent>().translation);
            n_Entity["Transformation"]["TranslationOffset"] = NodeVec3(entity.Get<TransformComponent>().translation_offset);
            n_Entity["Transformation"]["Rotation"]          = NodeVec3(entity.Get<TransformComponent>().rotation);
            n_Entity["Transformation"]["RotationOffset"]    = NodeVec3(entity.Get<TransformComponent>().rotation_offet);
            n_Entity["Transformation"]["Scale"]             = NodeVec3(entity.Get<TransformComponent>().scale);
            n_Entity["Transformation"]["ScaleOffset"]       = NodeVec3(entity.Get<TransformComponent>().scale_offset);
        }
    }
    template <> void SceneLoader::SaveComponent<RenderMeshComponent>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<RenderMeshComponent>())
        {
            n_Entity["RenderModel"] = (uint64_t)entity.Get<RenderMeshComponent>().id;
        }
    }
    template <> void SceneLoader::SaveComponent<ShaderComponent>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<ShaderComponent>())
        {
            n_Entity["Shader"] = (uint64_t)entity.Get<ShaderComponent>().id;
        }
    }
    template <> void SceneLoader::SaveComponent<FPSCamera>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<FPSCamera>())
        {
            n_Entity["FPSCamera"]["FOV"]       = entity.Get<FPSCamera>().m_FOV;
            n_Entity["FPSCamera"]["Aspect"]    = entity.Get<FPSCamera>().m_Aspect;
            n_Entity["FPSCamera"]["NearPlane"] = entity.Get<FPSCamera>().m_NearPlane;
            n_Entity["FPSCamera"]["FarPlane"]  = entity.Get<FPSCamera>().m_FarPlane;
            n_Entity["FPSCamera"]["Position"]  = NodeVec3(entity.Get<FPSCamera>().m_Position);
            n_Entity["FPSCamera"]["Direction"] = NodeVec3(entity.Get<FPSCamera>().m_Direction);
        }
    }
    template <> void SceneLoader::SaveComponent<LightSource>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<LightSource>())
        {
            n_Entity["LightSource"]["Type"]      = LightSourceTypeToString(entity.Get<LightSource>().type);
            n_Entity["LightSource"]["Ambient"]   = NodeVec3(entity.Get<LightSource>().ambient);
            n_Entity["LightSource"]["Diffuse"]   = NodeVec3(entity.Get<LightSource>().diffuse);
            n_Entity["LightSource"]["Specular"]  = NodeVec3(entity.Get<LightSource>().specular);
            n_Entity["LightSource"]["Direction"] = NodeVec3(entity.Get<LightSource>().direction);
            n_Entity["LightSource"]["Position"]  = NodeVec3(entity.Get<LightSource>().position);
            n_Entity["LightSource"]["CLQ"]       = NodeVec3(entity.Get<LightSource>().clq);
            n_Entity["LightSource"]["InnerCone"] = entity.Get<LightSource>().inner_cone_cos;
            n_Entity["LightSource"]["OuterCone"] = entity.Get<LightSource>().outer_cone_cos;
        }
    }
    template <> void SceneLoader::SaveComponent<SkyBox>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<SkyBox>())
        {
            n_Entity["SkyBox"] = (uint64_t)entity.Get<SkyBox>().id;
        }
    }
    template <> void SceneLoader::SaveComponent<ScriptComponent>(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.Has<ScriptComponent>())
        {
            auto& script_instance      = entity.Get<ScriptComponent>();
            n_Entity["Script"]["UUID"] = (uint64_t)script_instance.ID();
            for (auto& [name, field] : script_instance->GetFields())
            {
                n_Entity["Script"]["Fields"][name] = NodeScriptField(field);
            }
        }
    }

    void SceneLoader::SaveEntity(YAML::Node& n_Entity, Entity entity)
    {
        SaveComponent<TagComponent>(n_Entity, entity);
        SaveComponent<IdComponent>(n_Entity, entity);
        SaveComponent<TransformComponent>(n_Entity, entity);
        SaveComponent<RenderMeshComponent>(n_Entity, entity);
        SaveComponent<ShaderComponent>(n_Entity, entity);
        SaveComponent<FPSCamera>(n_Entity, entity);
        SaveComponent<LightSource>(n_Entity, entity);
        SaveComponent<SkyBox>(n_Entity, entity);
        SaveComponent<ScriptComponent>(n_Entity, entity);
    }
    YAML::Node SceneLoader::SaveTextures()
    {
        YAML::Node                n_Textures;
        std::vector<TextureAsset> textures = AssetManager::GetAllAssets<TextureAsset>();
        std::sort(textures.begin(), textures.end(), [](const TextureAsset& a, const TextureAsset& b) { return a.name < b.name; });

        for (const auto& texture : textures)
        {
            YAML::Node n_Texture;

            n_Textures[texture.name] = n_Texture;
            n_Texture["Path"]        = RelativeToExecutable(texture.loading_props.path).string();
            n_Texture["FlipUV"]      = texture.loading_props.flip_uvs;
            n_Texture["UUID"]        = (uint64_t)texture.id;
        }

        return n_Textures;
    }
    YAML::Node SceneLoader::SaveModels()
    {
        YAML::Node                   n_Models;
        std::vector<RenderMeshAsset> meshes = AssetManager::GetAllAssets<RenderMeshAsset>();
        std::sort(meshes.begin(), meshes.end(), [](const RenderMeshAsset& a, const RenderMeshAsset& b) { return a.name < b.name; });

        for (const auto& model : meshes)
        {
            YAML::Node n_Model;

            n_Models[model.name] = n_Model;
            n_Model["Path"]      = RelativeToExecutable(model.loading_props.path).string();
            n_Model["Compress"]  = model.loading_props.compress;
            n_Model["FlipUV"]    = model.loading_props.flip_uvs;
            n_Model["UUID"]      = (uint64_t)model.id;
        }

        return n_Models;
    }
    YAML::Node SceneLoader::SaveScripts()
    {
        YAML::Node               n_Scripts;
        std::vector<ScriptAsset> scripts = AssetManager::GetAllAssets<ScriptAsset>();
        std::sort(scripts.begin(), scripts.end(), [](const ScriptAsset& a, const ScriptAsset& b) { return a.path < b.path; });

        for (const auto& script : scripts)
        {
            if (script.name == "")
            {
                continue;
            }
            YAML::Node n_Script;

            n_Scripts[script.name] = n_Script;
            n_Script["Path"]       = RelativeToExecutable(script.path).string();
            n_Script["UUID"]       = (uint64_t)script.id;
        }

        return n_Scripts;
    }
    YAML::Node SceneLoader::SaveShaders()
    {
        YAML::Node               n_Shaders;
        std::vector<ShaderAsset> shaders = AssetManager::GetAllAssets<ShaderAsset>();
        std::sort(shaders.begin(), shaders.end(), [](const ShaderAsset& a, const ShaderAsset& b) { return a.name < b.name; });

        for (const auto& shader : shaders)
        {
            YAML::Node n_Shader;

            n_Shaders[shader.name] = n_Shader;
            n_Shader["UUID"]       = (uint64_t)shader.id;
            for (const auto& part : shader.parts)
            {
                n_Shader[ShaderPartTypeToString(part.type)] = RelativeToExecutable(part.path).string();
            }
        }
        return n_Shaders;
    }
    YAML::Node SceneLoader::SaveNode(Ref<SceneHierarchyNode> node)
    {
        YAML::Node res;
        if (node->IsEntity())
        {
            YAML::Node entity = res["Entity"];
            SaveEntity(entity, node->GetEntity());
        }
        else
        {
            res["Node"]["Name"] = node->GetName();

            auto childs = res["Node"]["Childs"];
            for (auto child : *node)
            {
                childs.push_back(SaveNode(child));
            }
        }
        return res;
    }

    void SceneLoader::Save(Ref<Scene> scene, const Path& path)
    {
        YAML::Node    n_Root, n_Scene, n_Assets;
        std::ofstream output_file;

        output_file.open(path);
        // TODO: Switch to throw.
        DE_ASSERT(output_file.is_open(), "Failed to open file to save scene.");

        n_Root["Scene"]   = n_Scene;
        n_Scene["Name"]   = scene->GetName();
        n_Scene["Assets"] = n_Assets;

        n_Assets["Models"]   = SaveModels();
        n_Assets["Shaders"]  = SaveShaders();
        n_Assets["Textures"] = SaveTextures();
        n_Assets["Scripts"]  = SaveScripts();
        for (auto child : *scene->GetHierarchy())
        {
            n_Scene["Hierarchy"].push_back(SaveNode(child));
        }

        output_file << n_Root;
    }

    //*~~~Loading~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Vec2 GetVec2(YAML::Node node)
    {
        Vec2 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();

        return res;
    }
    Vec3 GetVec3(YAML::Node node)
    {
        Vec3 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();
        res.z = node[2].as<float>();

        return res;
    }
    Vec4 GetVec4(YAML::Node node)
    {
        Vec4 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();
        res.z = node[2].as<float>();
        res.w = node[3].as<float>();

        return res;
    }
    void GetField(YAML::Node node, ScriptField& field)
    {
        switch (field.GetType())
        {
            case ScriptFieldType::Float: field.Get<float>() = node.as<float>(); break;
            case ScriptFieldType::Double: field.Get<double>() = node.as<double>(); break;
            case ScriptFieldType::Bool: field.Get<bool>() = node.as<bool>(); break;
            case ScriptFieldType::String: field.Get<std::string>() = node.as<std::string>(); break;
            case ScriptFieldType::I32: field.Get<int32_t>() = node.as<int32_t>(); break;
            case ScriptFieldType::I64: field.Get<int64_t>() = node.as<int64_t>(); break;
            case ScriptFieldType::UI32: field.Get<uint32_t>() = node.as<uint32_t>(); break;
            case ScriptFieldType::UI64: field.Get<uint64_t>() = node.as<uint64_t>(); break;
            case ScriptFieldType::Vec2: field.Get<Vec2>() = GetVec2(node); break;
            case ScriptFieldType::Vec3: field.Get<Vec3>() = GetVec3(node); break;
            case ScriptFieldType::Vec4: field.Get<Vec4>() = GetVec4(node); break;
            default: break;
        }
    }

    template <typename ComponentType> void SceneLoader::LoadComponent(YAML::Node n_Component, Entity& entity)
    {
        LOG_WARNING(StrCat("Load function of ", DemangleName<ComponentType>(), " undefined."), "SceneLoader");
    }
    template <> void SceneLoader::LoadComponent<TagComponent>(YAML::Node n_Component, Entity& entity)
    {
        entity.AddComponent<TagComponent>(n_Component.as<std::string>());
    }
    template <> void SceneLoader::LoadComponent<IdComponent>(YAML::Node n_Component, Entity& entity)
    {
        entity.AddComponent<IdComponent>(IdComponent(UUID(n_Component.as<uint64_t>())));
    }
    template <> void SceneLoader::LoadComponent<TransformComponent>(YAML::Node n_Component, Entity& entity)
    {
        TransformComponent transformation;

        transformation.translation        = GetVec3(n_Component["Translation"]);
        transformation.translation_offset = GetVec3(n_Component["TranslationOffset"]);
        transformation.rotation           = GetVec3(n_Component["Rotation"]);
        transformation.rotation_offet     = GetVec3(n_Component["RotationOffset"]);
        transformation.scale              = GetVec3(n_Component["Scale"]);
        transformation.scale_offset       = GetVec3(n_Component["ScaleOffset"]);

        entity.AddComponent(transformation);
    }
    template <> void SceneLoader::LoadComponent<RenderMeshComponent>(YAML::Node n_Component, Entity& entity)
    {
        UUID id = n_Component.as<uint64_t>();
        if (!ResourceManager::HasResource<RenderMesh>(id))
        {
            ResourceManager::AddResource<RenderMesh>(AssetManager::GetAsset<RenderMeshAsset>(id));
        }
        entity.AddComponent<RenderMeshComponent>({id, nullptr});
    }
    template <> void SceneLoader::LoadComponent<ShaderComponent>(YAML::Node n_Component, Entity& entity)
    {
        UUID id = n_Component.as<uint64_t>();
        if (!ResourceManager::HasResource<Shader>(id))
        {
            ResourceManager::AddResource<Shader>(AssetManager::GetAsset<ShaderAsset>(id));
        }
        entity.AddComponent<ShaderComponent>({id, ResourceManager::GetResource<Shader>(id)});
        m_Scene->m_RenderData->RequestShader(id);
    }
    template <> void SceneLoader::LoadComponent<FPSCamera>(YAML::Node n_Component, Entity& entity)
    {
        FPSCamera fps_camera;

        fps_camera.SetFov(n_Component["FOV"].as<float>());
        fps_camera.SetAspect(n_Component["Aspect"].as<float>());
        fps_camera.SetNearPlane(n_Component["NearPlane"].as<float>());
        fps_camera.SetFarPlane(n_Component["FarPlane"].as<float>());
        fps_camera.SetPos(GetVec3(n_Component["Position"]));
        fps_camera.SetDir(GetVec3(n_Component["Direction"]));

        entity.AddComponent<FPSCamera>(fps_camera);
    }
    template <> void SceneLoader::LoadComponent<LightSource>(YAML::Node n_Component, Entity& entity)
    {
        LightSource light_source;

        light_source.type           = StringToLightSourceType(n_Component["Type"].as<std::string>());
        light_source.ambient        = GetVec3(n_Component["Ambient"]);
        light_source.diffuse        = GetVec3(n_Component["Diffuse"]);
        light_source.specular       = GetVec3(n_Component["Specular"]);
        light_source.direction      = GetVec3(n_Component["Direction"]);
        light_source.position       = GetVec3(n_Component["Position"]);
        light_source.clq            = GetVec3(n_Component["CLQ"]);
        light_source.inner_cone_cos = n_Component["InnerCone"].as<float>();
        light_source.outer_cone_cos = n_Component["OuterCone"].as<float>();

        entity.AddComponent<LightSource>(light_source);
    }
    template <> void SceneLoader::LoadComponent<SkyBox>(YAML::Node n_Component, Entity& entity)
    {
        SkyBox box;
        box.id = n_Component.as<uint64_t>();
        if (!ResourceManager::HasResource<CubeMap>(box.id))
        {
            ResourceManager::AddResource<CubeMap>(AssetManager::GetAsset<TextureAsset>(box.id));
        }
        box.map = ResourceManager::GetResource<CubeMap>(box.id);

        entity.AddComponent<SkyBox>(box);
    }
    template <> void SceneLoader::LoadComponent<ScriptComponent>(YAML::Node n_Component, Entity& entity)
    {
        ScriptComponent script = ScriptEngine::CreateScript(n_Component["UUID"].as<uint64_t>());
        entity.AddComponent<ScriptComponent>(script);
        if (script.Valid())
        {
            for (auto& [name, field] : script->GetFields())
            {
                if (n_Component["Fields"][name])
                {
                    GetField(n_Component["Fields"][name], field);
                }
            }
        }
        else
        {
            LOG_INFO(StrCat("Failed to create valid script: ", std::to_string(script.ID())), "SceneLoader");
        }
    }

    void SceneLoader::LoadShaders(YAML::Node n_Shaders)
    {
        for (const auto& n_Shader : n_Shaders)
        {
            ShaderAsset asset;

            asset.id   = n_Shader.second["UUID"].as<uint64_t>();
            asset.name = n_Shader.first.as<std::string>();

            for (const auto& n_Part : n_Shader.second)
            {
                if (n_Part.first.as<std::string>() != "UUID")
                {
                    ShaderPart shader_part;
                    shader_part.type = StringToShaderPartType(n_Part.first.as<std::string>());
                    shader_part.path = fs::canonical(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / n_Part.second.as<std::string>());
                    asset.parts.push_back(shader_part);
                }
            }

            AssetManager::AddAsset(asset);
            ResourceManager::AddResource<Shader>(asset);
            m_Scene->m_RenderData->RequestShader(asset.id);
        }
    }
    void SceneLoader::LoadTextures(YAML::Node n_Textures)
    {
        for (const auto& n_Texture : n_Textures)
        {
            TextureAsset asset;

            asset.id   = n_Texture.second["UUID"].as<uint64_t>();
            asset.name = n_Texture.first.as<std::string>();

            asset.loading_props.flip_uvs = n_Texture.second["FlipUV"].as<bool>();
            asset.loading_props.path     = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / n_Texture.second["Path"].as<std::string>();

            asset.texture_data = TextureLoader::Load(asset.loading_props);

            AssetManager::AddAsset(asset);
        }
    }
    void SceneLoader::LoadModels(YAML::Node n_Models)
    {
        for (const auto& node : n_Models)
        {
            RenderMeshAsset asset;
            asset.name                   = node.first.as<std::string>();
            asset.id                     = node.second["UUID"].as<uint64_t>();
            asset.loading_props.compress = node.second["Compress"].as<bool>();
            asset.loading_props.flip_uvs = node.second["FlipUV"].as<bool>();
            asset.loading_props.path     = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / node.second["Path"].as<std::string>();
            asset.mesh_data              = ModelLoader::Load(asset.loading_props);
            AssetManager::AddAsset(asset);
        }
    }
    void SceneLoader::LoadScripts(YAML::Node n_Scripts)
    {
        for (const auto& node : n_Scripts)
        {
            ScriptAsset asset;

            asset.id   = node.second["UUID"].as<uint64_t>();
            asset.name = node.first.as<std::string>();
            asset.path = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / node.second["Path"].as<std::string>();
            AssetManager::AddAsset(asset);
            ScriptEngine::AddScript(asset);
        }
    }
    void SceneLoader::LoadAssets(YAML::Node n_Assets)
    {
        LoadShaders(n_Assets["Shaders"]);
        LoadModels(n_Assets["Models"]);
        LoadTextures(n_Assets["Textures"]);
        LoadScripts(n_Assets["Scripts"]);
    }
    Entity SceneLoader::LoadEntity(YAML::Node n_Entity)
    {
        Entity entity = m_Scene->CreateEmptyEntity();

        if (n_Entity["Tag"]) LoadComponent<TagComponent>(n_Entity["Tag"], entity);
        if (n_Entity["UUID"]) LoadComponent<IdComponent>(n_Entity["UUID"], entity);
        if (n_Entity["Transformation"]) LoadComponent<TransformComponent>(n_Entity["Transformation"], entity);
        if (n_Entity["RenderModel"]) LoadComponent<RenderMeshComponent>(n_Entity["RenderModel"], entity);
        if (n_Entity["Shader"]) LoadComponent<ShaderComponent>(n_Entity["Shader"], entity);
        if (n_Entity["FPSCamera"]) LoadComponent<FPSCamera>(n_Entity["FPSCamera"], entity);
        if (n_Entity["LightSource"]) LoadComponent<LightSource>(n_Entity["LightSource"], entity);
        if (n_Entity["SkyBox"]) LoadComponent<SkyBox>(n_Entity["SkyBox"], entity);
        if (n_Entity["Script"]) LoadComponent<ScriptComponent>(n_Entity["Script"], entity);

        if (entity.Has<RenderMeshComponent>() && entity.Has<ShaderComponent>())
        {
            uint64_t mesh_id   = entity.Get<RenderMeshComponent>().id;
            uint64_t shader_id = entity.Get<ShaderComponent>().id;

            entity.Get<RenderMeshComponent>().mesh_instance = m_Scene->m_RenderData->GetRenderMeshInstance(mesh_id, shader_id);
        }

        m_Scene->UpdateEmptyEntity(entity);
        return entity;
    }
    void SceneLoader::LoadHierarchyNode(YAML::Node n_Array, Ref<SceneHierarchyNode> load_to)
    {
        for (const auto& node : n_Array)
        {
            if (node["Entity"])
            {
                Entity entity = LoadEntity(node["Entity"]);
                load_to->AttachChild(CreateRef<SceneHierarchyNode>(entity.Get<TagComponent>(), entity));
            }
            if (node["Node"])
            {
                auto h_node = CreateRef<SceneHierarchyNode>(node["Node"]["Name"].as<std::string>());
                load_to->AttachChild(h_node);
                LoadHierarchyNode(node["Node"]["Childs"], h_node);
            }
        }
    }

    void SceneLoader::Load(Ref<Scene>& scene, const Path& path)
    {
        YAML::Node n_Root, n_Scene, n_Assets, n_Entities;
        scene = CreateRef<Scene>("NewScene");

        m_Scene = scene;

        n_Root   = YAML::LoadFile(path.string());
        n_Scene  = n_Root["Scene"];
        n_Assets = n_Scene["Assets"];
        n_Root   = n_Scene["Hierarchy"];

        LoadAssets(n_Assets);

        LoadHierarchyNode(n_Root, scene->m_HierarchyRoot);

        m_Scene = nullptr;
    }

    std::vector<ScriptAsset> SceneLoader::GetScriptAssets(const Path& path)
    {
        LOG_INFO("Getting script assets...", "SceneLoader");

        std::vector<ScriptAsset> result;
        YAML::Node               n_Root, n_Scripts;

        n_Root    = YAML::LoadFile(path.string());
        n_Scripts = n_Root["Scene"]["Assets"]["Scripts"];

        for (const auto& node : n_Scripts)
        {
            ScriptAsset asset;
            asset.id   = node.second["UUID"].as<uint64_t>();
            asset.name = node.first.as<std::string>();
            asset.path = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / node.second["Path"].as<std::string>();
            result.push_back(asset);
        }

        return result;
    }
}  // namespace DE