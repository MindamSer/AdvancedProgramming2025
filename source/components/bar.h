#pragma once


class Bar
{
public:
    Bar(int maxValue)
        : current(maxValue), max(maxValue) {}

    void change(int delta)
    {
        current += delta;
        if (current > max) current = max;
        if (current < 0) current = 0;
    }

    int getValue() const { return current; }
    int getMax() const { return max; }

protected:
    int current;
    int max;
};

class HealthBar : public Bar {};

class StaminaBar : public Bar
{
public:
    bool isDepleted() const { return current <= 0; }
    float getSpeed() const { return 5.0f + (isDepleted() ? 0.0 : 5.0); }
};
