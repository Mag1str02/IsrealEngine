#pragma once

#include "Addition/Base.h"
#include "Core/Scene/Components.h"
#include "Core/Scene/Scene.h"

namespace DE
{
    YAML::Node NodeVec2(Vec2 vec);
    YAML::Node NodeVec3(Vec3 vec);
    YAML::Node NodeVec4(Vec4 vec);

    class SceneLoader
    {
    public:
        static void Save(Ref<Scene> scene, Path path);
        static void Load(Ref<Scene>& scene, Path path);

    private:
        // TODO: Think how user can create custom load and save functions for own components.

        template <typename ComponentType> static void SaveComponent(YAML::Node n_Entity, Entity entity);
        template <typename ComponentType> static void LoadComponent(YAML::Node n_Component, Entity& entity);

        static void SaveEntity(YAML::Node n_Entity, Entity entity);
        static YAML::Node SaveEntities(Ref<Scene> scene);
        static YAML::Node SaveModels();
        static YAML::Node SaveShaders();
        static YAML::Node SaveAssets();

        static void LoadShaders(YAML::Node n_Shaders);
        static void LoadModels(YAML::Node n_Models);
        static void LoadAssets(YAML::Node n_Assets);
        static void LoadEntity(YAML::Node n_Entity);
        static void LoadEntities(YAML::Node n_Entities);

        static Ref<Scene> m_Scene;
    };

}  // namespace DE