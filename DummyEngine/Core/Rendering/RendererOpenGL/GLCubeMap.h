#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"

namespace DE {
    class GLCubeMap : public CubeMap {
    public:
        GLCubeMap(const Ref<TextureData> data);
        GLCubeMap(U32 size, TextureFormat format, TextureChannels channels, bool gen_mipmap);
        virtual ~GLCubeMap();
        virtual void Bind(U32 slot) const override;
        virtual float& GetLOD() override;

        GLCubeMap()                                  = delete;
        GLCubeMap(const GLCubeMap& other)            = delete;
        GLCubeMap(GLCubeMap&& other)                 = delete;
        GLCubeMap& operator=(const GLCubeMap& other) = delete;
        GLCubeMap& operator=(GLCubeMap&& other)      = delete;

    private:
        friend class GLFrameBuffer;
        float    m_LOD = 0;
        GLuint m_MapId;
    };
}  // namespace DE