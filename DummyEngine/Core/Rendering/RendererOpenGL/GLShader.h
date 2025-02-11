#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {
    namespace fs = std::filesystem;

    class GLShader : public Shader {
        LOG_AUTHOR(GLShader)
    public:
        GLShader()                               = delete;
        GLShader(const Shader& other)            = delete;
        GLShader(Shader&& other)                 = delete;
        GLShader& operator=(Shader&& other)      = delete;
        GLShader& operator=(const Shader& other) = delete;

        explicit GLShader(const std::vector<ShaderPart>& shader_parts);
        virtual ~GLShader();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void SetFloat(const std::string& uniform_name, float value) const override;
        virtual void SetFloat2(const std::string& uniform_name, float x, float y) const override;
        virtual void SetFloat3(const std::string& uniform_name, float x, float y, float z) const override;
        virtual void SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const override;
        virtual void SetFloat2(const std::string& uniform_name, Vec2 value) const override;
        virtual void SetFloat3(const std::string& uniform_name, Vec3 value) const override;
        virtual void SetFloat4(const std::string& uniform_name, Vec4 value) const override;
        virtual void SetInt(const std::string& uniform_name, int x) const override;
        virtual void SetInt2(const std::string& uniform_name, int x, int y) const override;
        virtual void SetInt3(const std::string& uniform_name, int x, int y, int z) const override;
        virtual void SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const override;
        virtual void SetMat4(const std::string& uniform_name, Mat4 value) const override;
        virtual void SetUnifromBlock(const std::string& uniform_name, U32 id) const override;

    private:
        static std::string ReadPartFromFile(const Path& path_to_file);

        void AddPart(ShaderPart part);

        std::vector<GLuint> parts_;
        GLuint              shader_id_;
    };
}  // namespace DummyEngine