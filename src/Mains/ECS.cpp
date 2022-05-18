#include "../Engine/Addition/de_lib.h"

using namespace DE;

struct Health
{
    double health;
    Health() {}
    Health(double h) : health(h) {}
};
struct Healing
{
    double heal_per_sec;
    Healing() {}
    Healing(double h) : heal_per_sec(h) {}
};

class HealingSystem : public System
{

public:
    HealingSystem()
    {
    }
    void Update(double dt) override
    {
        auto &health_array = GetComponentArray<Health>();
        auto &healing_array = GetComponentArray<Healing>();
        for (auto it : health_array)
        {
            if (healing_array.HasComponent(it.first))
            {
                it.second.health += dt * healing_array[it.first].heal_per_sec;
            }
        }
    }
};

void Test1()
{
    deHint(H_MAX_ENTITY_AMOUNT, 1000);
    deInitialize();
    Entity::Log();

    Entity *a = new Entity();
    Entity::Log();

    a->AddComponent<Health>();
    Entity::Log();

    std::vector<Entity> entities;
    for (size_t i = 0; i < 5; ++i)
    {
        entities.push_back(Entity());
        auto &h = entities.back().GetComponent<Health>();
        Entity::Log();
    }

    Entity b = *a;
    a->RemoveComponent<Health>();
    Entity::Log();

    delete a;
    entities[2].AddComponent<Transformation>();
    Entity::Log();

    deTerminate();
    Entity::Log();
}
void Test2()
{
    deInitialize();
    SystemManager::RegisterSystem<HealingSystem>();
    SystemManager::CalculateOrder();
    Entity::Log();

    std::vector<Entity> entities(5);
    std::string log_healing;
    std::string log_health;

    log_healing = "Healing: ";
    log_health = "Health: ";
    for (size_t i = 0; i < 5; ++i)
    {
        entities[i].AddComponent<Health>(Health(i * 10));
        entities[i].AddComponent<Healing>(Healing((5 - i) * 5));
        log_healing.append(std::to_string(entities[i].GetComponent<Healing>().heal_per_sec) + " ");
        log_health.append(std::to_string(entities[i].GetComponent<Health>().health) + " ");
    }
    entities[4].RemoveComponent<Healing>();
    Logger::Info("ECS", "Main", log_health);
    Logger::Info("ECS", "Main", log_healing);
    Entity::Log();

    SystemManager::Update(3);

    log_healing = "Healing: ";
    log_health = "Health: ";
    for (size_t i = 0; i < 5; ++i)
    {
        log_health.append(std::to_string(entities[i].GetComponent<Health>().health) + " ");
        if (i != 4)
        {
            log_healing.append(std::to_string(entities[i].GetComponent<Healing>().heal_per_sec) + " ");
        }
    }
    Logger::Info("ECS", "Main", log_health);
    Logger::Info("ECS", "Main", log_healing);
    Entity::Log();

    deTerminate();
    Entity::Log();
}
void Test3()
{
    deInitialize();
    Entity::Log();

    Entity a;
    Entity b;
    Entity c;
    Entity::Log();
    c = a;
    Entity::Log();
    c = b;
    Entity::Log();
    deTerminate();
    Entity::Log();
}

int main()
{
    Logger::Open(LOG_DIR / "ECS.txt", "ECS");
    Logger::Stage("ECS", "Main", "TEST 1");
    Test1();
    Logger::Stage("ECS", "Main", "TEST 2");
    Test2();
    Logger::Stage("ECS", "Main", "TEST 3");
    Test3();
    std::cout << "FINE!" << std::endl;
    return 0;
}
