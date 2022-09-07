#include "Core/Rendering/RendererOpenGL/GLCubeMap.h"
#include "Core/Rendering/RendererOpenGL/GLUtils.h"
#include "ToolBox/Editors/TextureEditor.h"

namespace DE
{
    GLCubeMap::GLCubeMap(const Ref<TextureData> data)
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_MapId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_MapId);

        // TODO: move somewhere else...

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for (size_t i = 0; i < 6; ++i)
        {
            auto side = TextureEditor::GetSkyBoxSide(data, CubeSide(i));
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         TextureFormatToGLTextureInternalFormat(side->Format()),
                         side->Width(),
                         side->Height(),
                         0,
                         TextureFormatToGLTextureInternalFormat(side->Format()),
                         GL_UNSIGNED_BYTE,
                         side->Data());
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    GLCubeMap::~GLCubeMap() { glDeleteTextures(1, &m_MapId); }

    void GLCubeMap::Bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_MapId); }
}  // namespace DE