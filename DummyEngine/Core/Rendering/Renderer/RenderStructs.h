#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    struct Material {
        enum class Type { None = 0, PBR };
        Type         type;
        Vec3         ambient;
        Vec3         albedo_color;
        Vec3         orm;
        float        shininess;
        Ref<Texture> albedo_map;
        Ref<Texture> normal_map;
        Ref<Texture> orm_map;

        void Apply(Ref<Shader> shader, const std::string& uniform_name = "u_Material") const;
    };

    struct RenderSubMesh {
        Material         material;
        Ref<VertexArray> vertex_array;

        RenderSubMesh Copy() const;
        void          FillData(const RenderSubMeshData& data);
    };

    class RenderMesh;
    class RenderMeshInstance {
    public:
        RenderMeshInstance() : m_Index(0), m_Mesh(nullptr) {}
        RenderMeshInstance(Ref<RenderMesh> mesh);
        ~RenderMeshInstance();

        void Bind(Ref<RenderMesh> mesh);
        void UnBind();

        template <typename Type> Type& at(U32 index);
        Ref<RenderMesh>                GetMesh();

    private:
        U32             m_Index;
        Ref<RenderMesh> m_Mesh;
    };
    class RenderMesh {
    public:
        RenderMesh() : m_InstanceBuffer(nullptr) {}
        RenderMesh(Ref<RenderMeshData> data);
        Ref<RenderMesh>             Copy() const;
        std::vector<RenderSubMesh>& GetSubMeshes();

        void UpdateInstanceBuffer();
        void SetInstanceBuffer(const BufferLayout& layout, U32 size);
        void FillData(Ref<RenderMeshData> data);

    private:
        friend class Renderer;
        friend class RenderMeshInstance;

        std::vector<RenderSubMesh>       m_SubMeshes;
        std::vector<RenderMeshInstance*> m_Instances;
        Ref<VertexBuffer>                m_InstanceBuffer;
    };
    template <typename Type> Type& RenderMeshInstance::at(U32 index) {
        return m_Mesh->m_InstanceBuffer->at(m_Index).Get<Type>(index);
    }

}  // namespace DE