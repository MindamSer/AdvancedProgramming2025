#pragma once

#include "math2d.h"
#include "path.h"

#include <vector>
#include <random>
#include <unordered_set>


class Dungeon
{
public:
    enum Tile
    {
        WALL,
        FLOOR
    };

    struct TileGrid
    {
    public:
        TileGrid(int x, int y)
            : width(x), height(y), tiles(x * y, WALL) {}

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getTileCount() const { return tiles.size(); }

        Tile &getTile(int x, int y) { return tiles[y * width + x]; }
        const Tile getTile(int x, int y) const { return tiles[y * width + x]; }

        int getFloorCount() const;
        int2 getNthTilePosition(int n, const Tile target) const;
        void setRectangle(int2 p0, int2 p1, const Tile target);
        void setLine(int2 p0, int2 p1, const Tile target);

    private:
        int width, height;
        std::vector<Tile> tiles;

        mutable int floorCount;
        mutable bool floorCounted;

        void countFloor() const;
    };

    struct Room
    {
        int x, y, w, h;
        int centerX() const { return x + w / 2; }
        int centerY() const { return y + h / 2; }
    };

public:
    Dungeon(int width, int height, int roomAttempts = 50)
        : grid(width, height) { generate(roomAttempts); }

    const TileGrid &getGrid() const { return grid; }

    int getDungeonWidth() const  { return grid.getWidth(); };
    int getDungeonHeight() const { return grid.getHeight(); };
    int2 getRandomFloorPosition() const { return grid.getNthTilePosition(std::uniform_int_distribution<int>(0, grid.getFloorCount() - 1)(randEng), FLOOR); };
    bool canPass(int2 coordinates) const { return grid.getTile(coordinates.x, coordinates.y) == Dungeon::FLOOR; }

    Path findPath(int2 from, int2 to, const std::unordered_set<int2> &forbiddenPositions) const;

private:
    mutable std::mt19937 randEng{ std::random_device{}() };

    TileGrid grid;
    std::vector<Room> rooms;

    void generate(const int roomAttempts);
    bool placeRoom(const Room& r);
    void connectRooms(const Room& a, const Room& b);
};
