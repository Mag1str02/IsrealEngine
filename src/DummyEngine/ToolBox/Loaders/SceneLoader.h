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
        static void Save(Ref<Scene> scene, const Path& path);
        static void Load(Ref<Scene>& scene, const Path& path);

    private:
        // TODO: Think how user can create custom load and save functions for own components.

        template <typename ComponentType> static void SaveComponent(YAML::Node& n_Entity, Entity entity);
        template <typename ComponentType> static void LoadComponent(YAML::Node n_Component, Entity& entity);

        static void       SaveEntity(YAML::Node& n_Entities, Entity entity);
        static YAML::Node SaveModels();
        static YAML::Node SaveTextures();
        static YAML::Node SaveShaders();
        static YAML::Node SaveAssets();
        static YAML::Node SaveNode(Ref<SceneHierarchyNode> node);

        static void   LoadShaders(YAML::Node n_Shaders);
        static void   LoadTextures(YAML::Node n_Textures);
        static void   LoadModels(YAML::Node n_Models);
        static void   LoadAssets(YAML::Node n_Assets);
        static Entity LoadEntity(YAML::Node n_Entity);
        static void   LoadHierarchyNode(YAML::Node n_Array, Ref<SceneHierarchyNode> load_to);

        static Ref<Scene> m_Scene;
    };

}  // namespace DE