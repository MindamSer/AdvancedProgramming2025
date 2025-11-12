#pragma once

#include "dungeon_generator.h"


class DungeonRestrictor
{
public:
    DungeonRestrictor(const Dungeon *_dungeon = nullptr)
        : dungeonPtr(_dungeon) {}

    void bindDungeon(const Dungeon *_dungeon) { dungeonPtr = _dungeon; }
    void clearDungeon() { dungeonPtr = nullptr; }

    bool canPass(int2 coordinates) const
    {
        if (!dungeonPtr)
            return false;

        const auto &grid = dungeonPtr->getGrid();
        const int x = coordinates.x;
        const int y = coordinates.y;

        if (x < 0 || y < 0 || y >= (int)grid.getHeight() || x >= (int)grid.getWidth())
            return false;

        return grid.getTile(x, y) == Dungeon::FLOOR;
    }

private:
    const Dungeon *dungeonPtr;
};
