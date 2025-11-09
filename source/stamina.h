#pragma once

#include "component.h"


class Stamina : public Component
{
public:
    Stamina(int maxStamina)
        : current(maxStamina), max(maxStamina) {}

    void change(int delta)
    {
        current += delta;
        if (current > max) current = max;
        if (current < 0) current = 0;
    }

    bool is_depleted() const { return current <= 0; }
    float get_speed() const { return 5.0f + (is_depleted() ? 0.0 : 5.0); }

public:
    int current;
    int max;
};