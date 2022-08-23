#include "Core/Scene/Components.h"

namespace DE
{
    IdComponent::IdComponent(UUID uuid) : id(uuid) {}

    TagComponent::TagComponent(const std::string& tag) : tag(tag) {}

    Mat4 TransformComponent::GetTransform() const
    {
        return glm::translate(glm::mat4(1.0f), translation + translation_offset) * glm::toMat4(glm::quat(glm::radians(rotation + rotation_offet))) *
               glm::scale(glm::mat4(1.0f), scale * scale_offset);
    }
}  // namespace DE