#pragma once

#include "component.h"
#include "world.h"
#include "health.h"

class StarvationSystem : public Component {
    float accumulator = 0.0f;
    const float damageInterval = 1.0f; // seconds
    const int damageAmount = 2; // health points
public:
    void on_update(float dt) override {
        accumulator += dt;
        if (accumulator < damageInterval) return;
        accumulator -= damageInterval;
        for (auto& obj : get_owner()->get_world()->get_objects()) {
            auto health = obj->get_component<Health>();
            if (health) {
                health->change(-damageAmount);
                if (health->current <= 0) {
                    get_owner()->get_world()->destroy_object(obj);
                }
            }
        }
    }
};