#pragma once
#include "component.h"
#include <memory>
#include <unordered_map>
#include <typeindex>

class World; // forward declaration

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
    template<typename T, typename... Args>
    std::shared_ptr<T> add_component(Args&&... args) {
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        components[std::type_index(typeid(T))] = comp;
        comp->owner = shared_from_this();
        comp->on_create();
        return comp;
    }
    template<typename T>
    std::shared_ptr<T> add_component(T *comp_ptr) {
        auto comp = std::shared_ptr<T>(comp_ptr);
        components[std::type_index(typeid(T))] = comp;
        comp->owner = shared_from_this();
        comp->on_create();
        return comp;
    }

    template<typename T>
    std::shared_ptr<T> get_component() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end())
            return std::static_pointer_cast<T>(it->second);
        return nullptr;
    }

    template<typename T>
    void remove_component() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end())
        {
            it->second->on_destroy();
            it->second.reset(); // components.erase(it); we can't remove this element while we can potentially iterating
        }
    }

    void update(float dt) {
        for (auto& [_, comp] : components) {
            comp->on_update(dt);
        }
    }

    ~GameObject() {
        for (auto& [_, comp] : components) {
            comp->on_destroy();
        }
    }

    World* get_world() const {
        return world.lock().get();
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    std::weak_ptr<World> world;
    friend class World;
};

using GameObjectPtr = std::shared_ptr<GameObject>;