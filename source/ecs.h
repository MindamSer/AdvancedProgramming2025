#pragma once

#include "components/transform2d.h"
#include "components/dungeon_restrictor.h"
#include "components/sprite.h"

#include <cstdint>
#include <tuple>
#include <vector>


struct Empty {};

#define COMPONENTS X(Transform2D) X(DungeonRestrictor) X(Sprite)
#define X(ARG) std::vector<ARG>,
using ComponentTuple = std::tuple<COMPONENTS Empty>;
#undef X


using EntityID = uint64_t;

struct EntityRegistry
{
    static ComponentTuple components;

    EntityRegistry() {}
};
