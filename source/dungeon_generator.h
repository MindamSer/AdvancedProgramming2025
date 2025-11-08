#pragma once

#include "math2d.h"

#include <vector>
#include <random>


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
        TileGrid(int x, int y) : w(x), h(y), tiles(x * y, WALL) {}

        const int getWidth() const { return w; }
        const int getHeight() const { return h; }
        const int getTileCount() const { return tiles.size(); }
        const int getFloorCount() const { return floorCount; }

        Tile &getTile(int x, int y) { return tiles[y * w + x]; }
        const Tile getTile(int x, int y) const { return tiles[y * w + x]; }

        const int2 getNthTilePosition(int n, const Tile target) const;
        void setRectangle(int2 p0, int2 p1, const Tile target);
        void setLine(int2 p0, int2 p1, const Tile target);
        void countFloor();

    private:
        int w, h, floorCount;
        std::vector<Tile> tiles;
    };

    struct Room
    {
        int x, y, w, h;
        int centerX() const { return x + w / 2; }
        int centerY() const { return y + h / 2; }
    };

    Dungeon(int width, int height, int roomAttempts = 50)
        : grid(width, height)
    { generate(roomAttempts); }

    const TileGrid &getGrid() const { return grid; }

    int getDungeonWidth() { return grid.getWidth(); };
    int getDungeonHeight() { return grid.getHeight(); };
    int2 getRandomFloorPosition();

private:
    std::mt19937 randEng{ std::random_device{}() };

    TileGrid grid;
    std::vector<Room> rooms;


    void generate(const int roomAttempts);

    bool placeRoom(const Room& r);
    void connectRooms(const Room& a, const Room& b);
};
