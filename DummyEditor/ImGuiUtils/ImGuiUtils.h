#pragma once

#include "DummyEditor/DummyEngineInclude.h"
#include "DummyEditor/ImGuiUtils/ImGuiIcons.h"

namespace DE {
    class ImGuiItem {
    public:
        ImGuiItem(const std::string& name) : m_Name(name) {}
        virtual ~ImGuiItem() = default;

        virtual void       OnImGui() = 0;
        void               Enable();
        void               Disable();
        bool&              GetController();
        const std::string& GetName() const;

    protected:
        bool        m_Controller = true;
        std::string m_Name       = "UnnamedItem";
    };

    namespace ImGuiUtils {
        namespace Constants {
            constexpr S32 BasicFontSize = 16;
            constexpr S32 ColorPading   = -1;
            constexpr S32 SliderPadding = -15;
        }  // namespace Constants

        void ClampRoundValue(Vec3& vec, float min, float max);
        void ClampRoundValue(Vec4& vec, float min, float max);

        constexpr ImVec4 U32ToColor(U32 color) {
            return {float((color & 0xFF0000) >> 16) / 255,
                    float((color & 0xFF00) >> 8) / 255,
                    float(color & 0xFF) / 255,
                    float((color & 0xFF000000) >> 24) / 255};
        }

        enum class PropertyType {
            None = 0,
            Color,
            Pos,
        };

        void EditProperty(std::string name, ImGuiDataType type, void* value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, float& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, double& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, U8& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, S8& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, U16& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, S16& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, U32& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, S32& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, U64& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, S64& value, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, bool& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, std::string& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, Vec2& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, Vec3& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, Vec4& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, ImVec4& vec, PropertyType property_type = PropertyType::None);
    }  // namespace ImGuiUtils

}  // namespace DE
