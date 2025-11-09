#pragma once

#include "dungeon_generator.h"
#include "restrictor.h"


class DungeonRestrictor : public IRestrictor
{
public:
    DungeonRestrictor(std::shared_ptr<Dungeon> dungeon)
        : dungeon(dungeon) {}

    bool can_pass(int2 coordinates) override
    {
        if (!dungeon)
            return false;
        const auto &grid = dungeon->getGrid();
        int x = coordinates.x;
        int y = coordinates.y;
        if (x < 0 || y < 0 || y >= (int)grid.getHeight() || x >= (int)grid.getWidth())
            return false;
        return grid.getTile(x, y) == Dungeon::FLOOR;
    }

public:
    std::shared_ptr<Dungeon> dungeon;
};
