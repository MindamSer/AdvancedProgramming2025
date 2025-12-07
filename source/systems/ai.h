#pragma once

#include "base.h"

#include "path.h"

#include "components/control.h"


constexpr size_t DUMMY_PATH_BATCH_SIZE = 8;


class AISysytem : BaseSystem
{
public:
    void process(float dt, EntityArchetype &entities) override
    {
        for (size_t i = 0; i < entities.size(); ++i)
        {
            if (entities.controls[i] != ControlID::AI)
                continue;

            const auto aiID = entities.aiModules[i];

            switch (aiID)
            {
                case AITypeID::DUMMY:
                {
                    auto &botPath = entities.paths[i];

                    if (botPath.empty())
                        for (size_t i = 0; i < DUMMY_PATH_BATCH_SIZE; ++i)
                            botPath.push_back(static_cast<Direction>(rand() % Direction::COUNT));
                }
                break;

                default:
                break;
            }
        }
    }
};
