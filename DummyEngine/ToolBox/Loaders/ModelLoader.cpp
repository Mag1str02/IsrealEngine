#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"

#include <assimp/postprocess.h>

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DE {

    ModelLoader::LoaderState ModelLoader::m_State;

    MaterialData ModelLoader::LoadMaterial(aiMaterial* mat) {
        MaterialData material;
        material.diffuse  = GetColor(mat, ColorType::Diffuse);
        material.specular = GetColor(mat, ColorType::Specular);
        material.ambient  = GetColor(mat, ColorType::Ambient);
        material.albedo   = GetColor(mat, ColorType::Albedo);
        material.orm      = GetColor(mat, ColorType::ORM);
        aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &material.shininess);

        material.albedo_map   = GetTexture(mat, aiTextureType_DIFFUSE);
        material.normal_map   = GetTexture(mat, aiTextureType_NORMALS);
        material.orm_map      = GetTexture(mat, aiTextureType_METALNESS);
        material.diffuse_map  = GetTexture(mat, aiTextureType_DIFFUSE);
        material.specular_map = GetTexture(mat, aiTextureType_SPECULAR);
        return material;
    }

    Ref<RenderMeshData> ModelLoader::Load(const RenderMeshAsset::LoadingProperties& properties) {
        m_State.m_Props    = properties;
        unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
        if (properties.flip_uvs) {
            flags |= aiProcess_FlipUVs;
        }
        const aiScene* scene = m_State.m_Importer.ReadFile(properties.path.string(), flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERROR("ModelLoader",
                      "Failed to load model (",
                      RelativeToExecutable(properties.path),
                      ") with error (",
                      m_State.m_Importer.GetErrorString(),
                      ")");
            return nullptr;
        }

        m_State.m_CurrentData = CreateScope<RenderMeshData>();

        m_State.m_CurrentMeshId    = 0;
        m_State.m_MeshesAmount     = 0;
        m_State.m_NodesAmount      = 0;
        m_State.m_VerticesAmount   = 0;
        m_State.m_CurrentDirectory = fs::canonical(properties.path / "..");

        ReadModelProperties(scene->mRootNode, scene);
        m_State.m_CurrentData->meshes.resize(m_State.m_MeshesAmount);
        ProcessNode(scene->mRootNode, scene);

        if (properties.compress) {
            m_State.m_CurrentData->Compress();
        }
        LOG_INFO("ModelLoader",
                 "Model loaded (",
                 RelativeToExecutable(properties.path),
                 ") with (",
                 m_State.m_MeshesAmount,
                 ") meshes, (",
                 m_State.m_VerticesAmount,
                 ") verticies");
        return m_State.m_CurrentData;
    }

    void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene) {
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);
        }
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
        RenderSubMeshData& current_mesh = m_State.m_CurrentData->meshes[m_State.m_CurrentMeshId];
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            Vertex3D vertex;

            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;

            if (mesh->mNormals) {
                vertex.normal.x = mesh->mNormals[i].x;
                vertex.normal.y = mesh->mNormals[i].y;
                vertex.normal.z = mesh->mNormals[i].z;
            }
            if (mesh->mTangents) {
                vertex.tangent.x = mesh->mTangents[i].x;
                vertex.tangent.y = mesh->mTangents[i].y;
                vertex.tangent.z = mesh->mTangents[i].z;
            }
            if (mesh->mTextureCoords[0]) {
                vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
                vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
            }
            current_mesh.vertices.push_back(vertex);
        }
        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; ++j) {
                current_mesh.indices.push_back(face.mIndices[j]);
            }
        }
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material       = scene->mMaterials[mesh->mMaterialIndex];
            current_mesh.material      = LoadMaterial(material);
            current_mesh.material.type = m_State.m_Props.mat_type;
        }
        ++m_State.m_CurrentMeshId;
    }
    Vec3 ModelLoader::GetColor(aiMaterial* mat, ColorType type) {
        aiColor3D color(1.f, 1.f, 1.f);
        switch (type) {
            case ColorType::Diffuse: mat->Get(AI_MATKEY_COLOR_DIFFUSE, color); break;
            case ColorType::Specular: mat->Get(AI_MATKEY_COLOR_SPECULAR, color); break;
            // case ColorType::Ambient: mat->Get(AI_MATKEY_COLOR_AMBIENT, color); break;
            case ColorType::ORM:
                mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, color.g);
                mat->Get(AI_MATKEY_METALLIC_FACTOR, color.b);
                break;
            default: break;
        }
        Vec3 res(color.r, color.g, color.b);
        return res;
    }
    Ref<TextureData> ModelLoader::GetTexture(aiMaterial* mat, aiTextureType type) {
        aiString file_name;
        Path     texture_path;
        if (mat->GetTextureCount(type) == 0) {
            return nullptr;
        } else if (mat->GetTextureCount(type) > 1) {
            LOG_WARNING("ModelLoader", "Model has more multiple textures of same type. Loading only first one.");
        }
        mat->GetTexture(type, 0, &file_name);

        if (!m_State.m_ModelTextures.contains(m_State.m_CurrentDirectory / file_name.C_Str())) {
            m_State.m_ModelTextures[m_State.m_CurrentDirectory / file_name.C_Str()] =
                TextureLoader::Load({m_State.m_CurrentDirectory / file_name.C_Str(), false});
        }
        return m_State.m_ModelTextures[m_State.m_CurrentDirectory / file_name.C_Str()];
    }
    void ModelLoader::ReadModelProperties(aiNode* node, const aiScene* scene) {
        ++m_State.m_NodesAmount;
        m_State.m_MeshesAmount += node->mNumMeshes;
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            m_State.m_VerticesAmount += scene->mMeshes[node->mMeshes[i]]->mNumVertices;
        }
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ReadModelProperties(node->mChildren[i], scene);
        }
    }
}  // namespace DE