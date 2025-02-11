#include "GLCubeMap.h"

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"
#include "DummyEngine/ToolBox/Editors/TextureEditor.h"

namespace DummyEngine {

    GLCubeMap::GLCubeMap(const Ref<TextureData> data) {
        glGenTextures(1, &map_id_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map_id_);

        // TODO: move somewhere else...
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        if (data->Width() % 4 != 0 || data->Height() % 3 != 0 || data->Width() / 4 != data->Height() / 3) {
            std::string error;
            if (data->Width() % 4 != 0) {
                error += "(Width % 4 != 0)";
            }
            if (data->Height() % 3 != 0) {
                error += "(Height % 3 != 0)";
            }
            if (data->Width() / 4 != data->Height() / 3) {
                error += "(Width / 4 != Height / 3)";
            }
            LOG_WARNING("Attempt to create cubemap with wrong texture size {}", error);
        }

        for (size_t i = 0; i < 6; ++i) {
            auto side = TextureEditor::GetSkyBoxSide(data, CubeSide(i));
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         GLTextureFormatInternal(Texture::DataFormat(side->Format()), Texture::DataChannels(side->Channels())),
                         side->Width(),
                         side->Height(),
                         0,
                         GLTextureFormatExternal(Texture::DataChannels(side->Channels())),
                         GLDataType(side->Format()),
                         side->Data());
        }
        glCheckError();
    }
    GLCubeMap::GLCubeMap(U32 size, Texture::Format format, Texture::Channels channels, bool gen_mipmap, bool depth_map) {
        glGenTextures(1, &map_id_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map_id_);

        // TODO: move somewhere else...
        if (!depth_map) {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (gen_mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for (size_t i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         GLTextureFormatInternal(format, channels),
                         size,
                         size,
                         0,
                         GLTextureFormatExternal(channels),
                         depth_map ? GL_FLOAT : GL_UNSIGNED_BYTE,
                         nullptr);
        }
        if (gen_mipmap) {
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        glCheckError();
    }
    GLCubeMap::~GLCubeMap() {
        glDeleteTextures(1, &map_id_);
    }

    float& GLCubeMap::GetLOD() {
        return lod_;
    }
    void GLCubeMap::Bind(U32 slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map_id_);
    }

}  // namespace DummyEngine