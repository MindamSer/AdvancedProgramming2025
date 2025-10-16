#pragma once

#include "component.h"

class Health : public Component {
public:
    int current;
    int max;

    Health(int maxHealth) : current(maxHealth), max(maxHealth) {}

    void change(int delta) {
        current += delta;
        if (current > max) current = max;
        if (current < 0) current = 0;
    }
};