#pragma once

#include <cmath>
#include <cstdint>


struct int2
{
    int x, y;
    int2(int x=0, int y=0)
        : x(x), y(y) {}

    int2 operator+() { return *this; }
    int2 operator-() { return {-x, -y}; }

    int2 &operator+=(const int2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int2 &operator-=(const int2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    int2 operator+(const int2 &other) const { return int2{x, y} += other; }

    int2 operator-(const int2 &other) const { return int2{x, y} -= other; }

    bool operator==(const int2 &other) const { return x == other.x && y == other.y; }

};

inline uint32_t manhattanDist(const int2 &a, const int2 &b) { return abs(a.x - b.x) + abs(a.y - b.y); }
inline float euclidDist(const int2 &a, const int2 &b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }

template<>
struct std::hash<int2>
{
    size_t operator()(const int2 &a) const { return std::hash<int>{}(a.x) << 32 | std::hash<int>{}(a.x) >> 32; }
};


struct float2
{
    float x, y;
    float2(float x=0, float y=0)
        : x(x), y(y) {}
};
