#pragma once

#include "DummyEngine/Core/Physics/ConvexCollider.hpp"
#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"

namespace DummyEngine {

    class ResourceManager : public Singleton<ResourceManager> {
        LOG_AUTHOR(ResourceManager)
        SINGLETON(ResourceManager)
    public:
        S_METHOD_DEF(bool, LoadShader, (UUID id));
        S_METHOD_DEF(bool, LoadRenderMesh, (UUID id));
        S_METHOD_DEF(bool, LoadCubeMap, (UUID id));
        S_METHOD_DEF(bool, LoadHitBox, (UUID id));
        S_METHOD_DEF(bool, LoadTexture, (UUID id));

        S_METHOD_DEF(std::optional<Ref<Shader>>, GetShader, (UUID id));
        S_METHOD_DEF(std::optional<Ref<RenderMesh>>, GetRenderMesh, (UUID id));
        S_METHOD_DEF(std::optional<Ref<CubeMap>>, GetCubeMap, (UUID id));
        S_METHOD_DEF(std::optional<Ref<Physics::ConvexHitbox>>, GetHitBox, (UUID id));
        S_METHOD_DEF(std::optional<Ref<Texture>>, GetTexture, (UUID id));

        S_METHOD_DEF(bool, HasShader, (UUID id));
        S_METHOD_DEF(bool, HasRenderMesh, (UUID id));
        S_METHOD_DEF(bool, HasCubeMap, (UUID id));
        S_METHOD_DEF(bool, HasHitBox, (UUID id));
        S_METHOD_DEF(bool, HasTexture, (UUID id));

        S_METHOD_DEF(bool, DeleteShader, (UUID id));
        S_METHOD_DEF(bool, DeleteRenderMesh, (UUID id));
        S_METHOD_DEF(bool, DeleteCubeMap, (UUID id));
        S_METHOD_DEF(bool, DeleteHitBox, (UUID id));
        S_METHOD_DEF(bool, DeleteTexture, (UUID id));

        S_METHOD_DEF(Unit, Clear, ());

    private:
        std::unordered_map<UUID, Ref<Shader>>                shaders_;
        std::unordered_map<UUID, Ref<RenderMesh>>            render_meshes_;
        std::unordered_map<UUID, Ref<CubeMap>>               cube_maps_;
        std::unordered_map<UUID, Ref<Texture>>               textures_;
        std::unordered_map<UUID, Ref<Physics::ConvexHitbox>> hit_boxes_;
    };

}  // namespace DummyEngine