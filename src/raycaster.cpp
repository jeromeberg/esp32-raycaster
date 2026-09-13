#include "raycaster.h"

#include <cmath>
#include <limits>

#include "config.h"

static RayHit noHit() { return {false, 0.0f, HitSide::NONE}; }

static float initialSideDistance(float position, int mapCoordinate,
                                 float rayDirection, float deltaDistance,
                                 int step) {
    if (step < 0)
        return (position - static_cast<float>(mapCoordinate)) * deltaDistance;
    return (static_cast<float>(mapCoordinate) + 1.0f - position) *
           deltaDistance;
}

RayHit castRay(const Map& map, const Player& player, std::uint8_t column) {
    if (column >= config::SCREEN_WIDTH) return noHit();
    const float cameraX = 2.0f * (static_cast<float>(column) + 0.5f) /
                              static_cast<float>(config::SCREEN_WIDTH) -
                          1.0f;
    const float rayDirectionX =
        player.direction.x + player.cameraPlane.x * cameraX;
    const float rayDirectionY =
        player.direction.y + player.cameraPlane.y * cameraX;
    const float infinity = std::numeric_limits<float>::infinity();
    const float deltaDistanceX =
        rayDirectionX == 0.0f ? infinity : std::fabs(1.0f / rayDirectionX);
    const float deltaDistanceY =
        rayDirectionY == 0.0f ? infinity : std::fabs(1.0f / rayDirectionY);
    int mapX = static_cast<int>(std::floor(player.position.x));
    int mapY = static_cast<int>(std::floor(player.position.y));
    const int stepX = rayDirectionX < 0.0f ? -1 : 1;
    const int stepY = rayDirectionY < 0.0f ? -1 : 1;
    float sideDistanceX = initialSideDistance(
        player.position.x, mapX, rayDirectionX, deltaDistanceX, stepX);
    float sideDistanceY = initialSideDistance(
        player.position.y, mapY, rayDirectionY, deltaDistanceY, stepY);
    HitSide side = HitSide::NONE;

    for (int step = 0; step < Map::WIDTH * Map::HEIGHT; ++step) {
        if (sideDistanceX < sideDistanceY) {
            sideDistanceX += deltaDistanceX;
            mapX += stepX;
            side = HitSide::X;
        } else {
            sideDistanceY += deltaDistanceY;
            mapY += stepY;
            side = HitSide::Y;
        }
        if (!map.isWall(mapX, mapY)) continue;
        float distance;
        if (side == HitSide::X) {
            distance = (static_cast<float>(mapX) - player.position.x +
                        static_cast<float>(1 - stepX) * 0.5f) /
                       rayDirectionX;
        } else {
            distance = (static_cast<float>(mapY) - player.position.y +
                        static_cast<float>(1 - stepY) * 0.5f) /
                       rayDirectionY;
        }
        if (!std::isfinite(distance) || distance <= 0.0f) return noHit();
        return {true, distance, side};
    }
    return noHit();
}
