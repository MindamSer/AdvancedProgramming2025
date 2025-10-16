#pragma once
#include <memory>

class GameObject; // forward declaration
class Component {
public:
    virtual void on_create() {}
    virtual void on_update(float dt) {}
    virtual void on_destroy() {}
    virtual ~Component() = default;
    std::shared_ptr<GameObject> get_owner() const {
        return owner.lock();
    }
private:
    std::weak_ptr<GameObject> owner;
    friend class GameObject;
};