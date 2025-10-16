#pragma once

#include "component.h"
#include "world.h"
#include "stamina.h"

class TirednessSystem : public Component {
    float accumulator = 0.0f;
    const float tirednessInterval = 1.0f; // seconds
    const int tirednessAmount = 5; // stamina points
public:
    void on_update(float dt) override {
        accumulator += dt;
        if (accumulator < tirednessInterval) return;
        accumulator -= tirednessInterval;
        for (auto& obj : get_owner()->get_world()->get_objects()) {
            auto stamina = obj->get_component<Stamina>();
            if (stamina) {
                stamina->change(-tirednessAmount);
            }
        }
    }
};