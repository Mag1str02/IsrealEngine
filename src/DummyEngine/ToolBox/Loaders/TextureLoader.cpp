#include <stb_image.h>

#include "ToolBox/Loaders/TextureLoader.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{
    TextureLoader::LoaderState TextureLoader::m_State;

    Ref<TextureData> TextureLoader::Load(const TextureLoadingProps& props)
    {
        unsigned char* stb_data;
        int width, height, nrChannels;
        TextureFormat format;
        std::string format_s;

        m_State.m_CurrentData = CreateRef<TextureData>();

        stbi_set_flip_vertically_on_load(props.flip ? true : false);

        stb_data = stbi_load(props.path.string().c_str(), &width, &height, &nrChannels, 0);

        if (!stb_data)
        {
            Logger::Error("loading", "TextureLoader", "Failed to load texture " + RelativeToExecutable(props.path).string());
            return nullptr;
        }

        switch (nrChannels)
        {
            case 1:
                format_s = "RED";
                format = TextureFormat::RED;
                break;
            case 3:
                format_s = "RGB";
                format = TextureFormat::RGB;
                break;
            case 4:
                format_s = "RGBA";
                format = TextureFormat::RGBA;
                break;
            default:
                format_s = "NONE";
                format = TextureFormat::None;
                break;
        }

        m_State.m_CurrentData->SetData(stb_data, width, height, format);
        stbi_image_free(stb_data);

        Logger::Info("loading", "TextureLoader", "Texture loaded " + RelativeToExecutable(props.path).string() + " Format (" + format_s + ")");
        return m_State.m_CurrentData;
    }
}  // namespace DE