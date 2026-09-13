#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

#include "display.h"
#include "map.h"
#include "player.h"

struct WallSlice {
    std::uint8_t startY;
    std::uint8_t height;
};

WallSlice calculateWallSlice(float perpendicularDistance);

void renderFrame(const Map& map, const Player& player, Display& display);

#endif
