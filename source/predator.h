#pragma once

#include "component.h"
#include "world.h"
#include "health.h"
#include "food_consumer.h"

class Predator : public Component {
public:
    void on_update(float dt) override {
        auto myTransform = get_owner()->get_component<Transform2D>();
        for (auto& obj : get_owner()->get_world()->get_objects()) {
            auto victim = obj->get_component<FoodConsumer>();
            if (victim) {
                // Simple collision check (assuming both have Transform2D)
                auto victimTransform = obj->get_component<Transform2D>();
                if (myTransform && victimTransform) {
                    if (int(myTransform->x) == int(victimTransform->x) &&
                        int(myTransform->y) == int(victimTransform->y)) {
                        auto victimHp = obj->get_component<Health>();
                        auto predatorHp = get_owner()->get_component<Health>();
                        if (victimHp && predatorHp) {
                            predatorHp->change(victimHp->current); // heal predator
                            get_owner()->get_world()->destroy_object(obj); // kill victim
                        }
                        break; // Consume only one victim at a time
                    }
                }
            }
        }
    }
};