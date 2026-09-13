#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <cstdint>

#include "map.h"
#include "player.h"

enum class HitSide : std::uint8_t { NONE, X, Y };

struct RayHit {
    bool hit;
    float perpendicularDistance;
    HitSide side;
};

RayHit castRay(const Map& map, const Player& player, std::uint8_t column);

#endif
