#pragma once

// #include "character.h"


// class NPC : public Character
// {
// public:
//     NPC(int maxHealth, int maxStamina, bool predator = false)
//         : Character(maxHealth, maxStamina), isPredator(predator) {}

//     void onCreate() override {};
//     void onUpdate(float dt) override
//     {
//         accumulatedTime += dt * stamina.getSpeed();
//         if (accumulatedTime < 1.0f)
//             return;
//         accumulatedTime -= 1.0f;

//         const int2 directions [] = {
//             int2{1,0},
//             int2{-1,0},
//             int2{0,1},
//             int2{0,-1}
//         };
//         int2 move = directions[rand() % 4];

//         if (restrictor.canPass(int2(static_cast<int>(position.x) + move.x, static_cast<int>(position.y) + move.y)))
//         {
//             position.x += move.x;
//             position.y += move.y;
//         }
//     }
//     void onDestroy() override {};

// private:
//     float accumulatedTime;
//     bool isPredator;
// };
