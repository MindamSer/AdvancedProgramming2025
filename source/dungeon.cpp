#include "dungeon.h"

#include <utility>
#include <cstdint>
#include <set>
#include <unordered_set>
#include <unordered_map>


constexpr int ROOM_MIN_WIDTH = 4;
constexpr int ROOM_MAX_WIDTH = 10;
constexpr int ROOM_MIN_HEIGHT = 4;
constexpr int ROOM_MAX_HEIGHT = 8;


int Dungeon::TileGrid::getFloorCount() const
{
    countFloor();
    return floorCount;
}

int2 Dungeon::TileGrid::getNthTilePosition(int n, const Tile target) const
{
    for (int i = 0; i < tiles.size(); ++i)
        if (tiles[i] == target)
        {
            if (n == 0)
                return int2{i % width, i / width};
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
            tiles[j * width + i] = target;

    floorCounted = false;
}

void Dungeon::TileGrid::setLine(int2 p0, int2 p1, const Tile target)
{
    if (p0.y == p1.y)
    {
        if (p0.x > p1.x)
            std::swap(p0, p1);
        for (int i = p0.x; i <= p1.x; ++i)
            tiles[p0.y * width + i] = target;

        floorCounted = false;

        return;
    }

    if (p0.x == p1.x)
    {
        if (p0.y > p1.y)
            std::swap(p0, p1);
        for (int j = p0.y; j <= p1.y; ++j)
            tiles[j * width + p0.x] = target;

        floorCounted = false;

        return;
    }
}

void Dungeon::TileGrid::countFloor() const
{
    if(floorCounted)
        return;

    floorCount = 0;
    for (Tile t : tiles)
        if (t == FLOOR)
            floorCount++;
    floorCounted = true;
}


Path Dungeon::findPath(int2 from, int2 to, const std::unordered_set<int2> &forbiddenPositions) const
{
    struct pathStep
    {
        int2 pos;
        uint32_t startDist;
        uint32_t finishDist;
    };

    struct pathComparator
    {
        bool operator()(const pathStep &a, const pathStep &b) const { return (a.startDist + a.finishDist) < (b.startDist + b.finishDist); }
    };


    // cleaning state
    static std::set<pathStep, pathComparator> openListOrdered;
    static std::unordered_set<int2> openList;
    static std::unordered_set<int2> closedList;
    static std::unordered_map<int2, Direction> dirToPrevPos;
    openListOrdered.clear();
    openList.clear();
    closedList.clear();
    dirToPrevPos.clear();


    // setting initial state
    for (uint32_t dir = 0; dir < Direction::COUNT; ++dir)
    {
        const int2 targetPos = from + directionVectors[dir];
        if (canPass(targetPos) && !forbiddenPositions.contains(targetPos))
        {
            openListOrdered.insert({
                targetPos,
                directionCosts[dir],
                manhattanDist(targetPos, to) * 10,
            });
            openList.insert(targetPos);
            dirToPrevPos.insert_or_assign(targetPos, static_cast<Direction>(Direction::OPP_SUM - dir));
        }
    }
    closedList.insert(from);


    // iterating over queued steps
    while (!openListOrdered.empty())
    {
        if (openListOrdered.begin()->finishDist != 0)
        {
            const pathStep curStep = *openListOrdered.begin();

            for (uint32_t dir = 0; dir < Direction::COUNT; ++dir)
            {
                const int2 targetPos = curStep.pos + directionVectors[dir];

                if (closedList.contains(targetPos))
                    continue;

                if (openList.contains(targetPos))
                {
                    auto openIter = openListOrdered.begin();
                    while (openIter->pos != targetPos)
                        ++openIter;
                    if (curStep.startDist + directionCosts[dir] < openIter->startDist)
                    {
                        openListOrdered.erase(openIter);
                        openListOrdered.insert({
                            targetPos,
                            curStep.startDist + directionCosts[dir],
                            manhattanDist(targetPos, to) * 10,
                        });
                        dirToPrevPos.insert_or_assign(targetPos, static_cast<Direction>(Direction::OPP_SUM - dir));
                    }
                    continue;
                }

                if (canPass(targetPos) && !forbiddenPositions.contains(targetPos))
                {
                    openListOrdered.insert({
                        targetPos,
                        curStep.startDist + directionCosts[dir],
                        manhattanDist(targetPos, to) * 10,
                    });
                    openList.insert(targetPos);
                    dirToPrevPos.insert_or_assign(targetPos, static_cast<Direction>(Direction::OPP_SUM - dir));
                }
            }

            openListOrdered.erase(openListOrdered.begin());
            openList.erase(curStep.pos);
            closedList.insert(curStep.pos);
        }
        else
        {
            openListOrdered.clear();
            openList.clear();
            closedList.insert(to);
        }
    }


    // if there is no direction from finish cell, algorithm didnt find a way to it
    if (dirToPrevPos.find(to) == dirToPrevPos.end())
        return {};

    // gathering path
    Path res;
    res.reserve(closedList.size());

    // going backwards from destination
    int2 curPos = to;
    while (curPos != from)
    {
        const Direction dir = dirToPrevPos[curPos];
        res.push_back(static_cast<Direction>(Direction::OPP_SUM - dir));
        curPos += directionVectors[dir];
    }
    // flipping to get path from starh
    for (auto front = res.begin(), rear = res.end() - 1; front < rear; ++front, --rear)
        std::swap(*front, *rear);

    return res;
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
