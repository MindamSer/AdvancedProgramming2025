#pragma once

#include <variant>


struct BaseFood
{
    BaseFood(int val=0)
        : value(val) {}

    int value;
};

struct HeathFood : BaseFood {};
struct StaminaFood : BaseFood {};

using Food = std::variant<HeathFood, StaminaFood>;
