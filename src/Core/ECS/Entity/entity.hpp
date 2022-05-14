#pragma once

#include <cinttypes>

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../Component/component_manager.hpp"
#include "entity_manager.hpp"


namespace DE {
using EntityId = int64_t;

class Entity {
private:
    friend class EntityManager;
    EntityId _id;

public:
    Entity() {
        _id = -1;
    }

    void Destroy() {
        EntityManager::Get().DestroyEntity(_id);
        _id = -1;
    }
    void Create() {
        EntityManager::Get().DestroyEntity(_id);
        _id = EntityManager::Get().CreateEntity();
    }
    bool valid() {
        return _id != -1;
    }
    template <typename ComponentType>
    void AddComponent(ComponentType component = ComponentType()) {
        ComponentManager::Get().AddComponent(_id, component);
    }

    template <typename ComponentType>
    void RemoveComponent() {
        ComponentManager::Get().RemoveComponent<ComponentType>(_id);
    }

    template <typename ComponentType>
    ComponentType* GetComponent() {
        return ComponentManager::Get().GetComponent<ComponentType>(_id);
    }
};
}  // namespace DE
