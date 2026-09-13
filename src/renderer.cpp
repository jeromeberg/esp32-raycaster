#include "renderer.h"

#include <cmath>

#include "config.h"
#include "raycaster.h"

WallSlice calculateWallSlice(float perpendicularDistance) {
    if (!std::isfinite(perpendicularDistance) ||
        perpendicularDistance <= 0.0f) {
        return {0, 0};
    }
    const float safeDistance =
        perpendicularDistance < 0.001f ? 0.001f : perpendicularDistance;
    int height = static_cast<int>(config::SCREEN_HEIGHT / safeDistance);
    if (height < 1) height = 1;
    if (height > config::SCREEN_HEIGHT) height = config::SCREEN_HEIGHT;
    const int startY = (config::SCREEN_HEIGHT - height) / 2;
    return {
        static_cast<std::uint8_t>(startY),
        static_cast<std::uint8_t>(height),
    };
}

void renderFrame(const Map& map, const Player& player, Display& display) {
    display.clear();
    for (std::uint8_t column = 0; column < config::SCREEN_WIDTH; ++column) {
        const RayHit hit = castRay(map, player, column);
        if (!hit.hit) continue;
        const WallSlice slice = calculateWallSlice(hit.perpendicularDistance);
        if (slice.height > 0)
            display.drawVerticalLine(column, slice.startY, slice.height);
    }
    display.present();
}
