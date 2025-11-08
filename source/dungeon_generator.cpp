#include "dungeon_generator.h"

#include <utility>


constexpr int ROOM_MIN_WIDTH = 4;
constexpr int ROOM_MAX_WIDTH = 10;
constexpr int ROOM_MIN_HEIGHT = 4;
constexpr int ROOM_MAX_HEIGHT = 8;


const int2  Dungeon::TileGrid::getNthTilePosition(int n, const Tile target) const
{
    for (int i = 0; i < tiles.size(); ++i)
        if (tiles[i] == target)
        {
            if (n == 0)
                return int2{i % w, i / w};
            --n;
        }

    return int2{-1, -1};
}

void Dungeon::TileGrid::setRectangle(int2 p0, int2 p1, const Tile target)
{
    if (p0.x > p1.x)
        std::swap(p0.x, p1.x);
    if (p0.y > p1.y)
        std::swap(p0.y, p1.y);

    for (int j = p0.y; j <= p1.y; ++j)
        for (int i = p0.x; i <= p1.x; ++i)
            tiles[j * w + i] = target;
}

void Dungeon::TileGrid::setLine(int2 p0, int2 p1, const Tile target)
{
    if (p0.y == p1.y)
    {
        if (p0.x > p1.x)
            std::swap(p0, p1);
        for (int i = p0.x; i <= p1.x; ++i)
            tiles[p0.y * w + i] = target;

        return;
    }

    if (p0.x == p1.x)
    {
        if (p0.y > p1.y)
            std::swap(p0, p1);
        for (int j = p0.y; j <= p1.y; ++j)
            tiles[j * w + p0.x] = target;

        return;
    }
}

void Dungeon::TileGrid::countFloor()
{
    floorCount = 0;
    for (Tile t : tiles)
        if (t == FLOOR)
            floorCount++;
}


int2 Dungeon::getRandomFloorPosition()
{
    // Возвращает случайную позицию напольного тайла
    // Не эффективно для больших карт, но сойдет для примера
    return grid.getNthTilePosition(std::uniform_int_distribution<int>(0, grid.getFloorCount() - 1)(randEng), FLOOR);
}

void Dungeon::generate(const int roomAttempts)
{
    std::uniform_int_distribution<int> rw(ROOM_MIN_WIDTH, ROOM_MAX_WIDTH);
    std::uniform_int_distribution<int> rh(ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT);
    std::uniform_int_distribution<int> rx(1, grid.getWidth() - ROOM_MAX_WIDTH - 2);
    std::uniform_int_distribution<int> ry(1, grid.getHeight() - ROOM_MAX_HEIGHT - 2);

    // Ставим комнаты
    for (int i = 0; i < roomAttempts; i++)
    {
        Room r {
            rx(randEng),
            ry(randEng),
            rw(randEng),
            rh(randEng)
        };
        if (placeRoom(r))
            rooms.push_back(r);
    }

    // Соединяем комнаты коридорами
    for (size_t i = 1; i < rooms.size(); i++)
        connectRooms(rooms[i - 1], rooms[i]);

    // Подсчитываем количество напольных тайлов (для getRandomFloorPosition)
    grid.countFloor();
}

bool Dungeon::placeRoom(const Room& r)
{
    // Проверка выхода за границы
    if (r.x < 1 || r.y < 1 || r.x + r.w >= grid.getWidth() - 1 || r.y + r.h >= grid.getHeight() - 1)
        return false;

    // Проверка пересечения
    for (int y = r.y - 1; y < r.y + r.h + 1; ++y)
        for (int x = r.x - 1; x < r.x + r.w + 1; ++x)
            if (grid.getTile(x, y) == FLOOR) return false;

    // Рисуем комнату
    grid.setRectangle({r.x, r.y}, {r.x + r.w, r.y + r.h}, FLOOR);

    return true;
}

void Dungeon::connectRooms(const Room& a, const Room& b)
{
    int x1 = a.centerX(), y1 = a.centerY();
    int x2 = b.centerX(), y2 = b.centerY();

    // Простейший L-образный коридор
    if (std::uniform_int_distribution<int>(0,1)(randEng))
    {
        grid.setLine({x1, y1}, {x1, y2}, FLOOR);
        grid.setLine({x1, y2}, {x2, y2}, FLOOR);
    }
    else
    {
        grid.setLine({x1, y1}, {x2, y1}, FLOOR);
        grid.setLine({x2, y1}, {x2, y2}, FLOOR);
    }
}
