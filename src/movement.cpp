#include "movement.h"

#include <cmath>

#include "config.h"

static float clampDeltaSeconds(float deltaSeconds) {
    if (deltaSeconds < 0.0f) return 0.0f;
    if (deltaSeconds > config::MAX_FRAME_SECONDS)
        return config::MAX_FRAME_SECONDS;
    return deltaSeconds;
}

static bool canMove(const Map& map, float x, float y) {
    const float radius = config::COLLISION_RADIUS;
    const int left = static_cast<int>(std::floor(x - radius));
    const int right = static_cast<int>(std::floor(x + radius));
    const int top = static_cast<int>(std::floor(y - radius));
    const int bottom = static_cast<int>(std::floor(y + radius));

    return !map.isWall(left, top) && !map.isWall(right, top) &&
           !map.isWall(left, bottom) && !map.isWall(right, bottom);
}

static void rotatePlayer(Player& player, float angle) {
    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);
    const Vector2 oldDirection = player.direction;
    const Vector2 oldPlane = player.cameraPlane;

    player.direction.x = oldDirection.x * cosine - oldDirection.y * sine;
    player.direction.y = oldDirection.x * sine + oldDirection.y * cosine;
    player.cameraPlane.x = oldPlane.x * cosine - oldPlane.y * sine;
    player.cameraPlane.y = oldPlane.x * sine + oldPlane.y * cosine;
}

static void moveForward(Player& player, const Map& map, float distance) {
    const float nextX = player.position.x + player.direction.x * distance;
    if (canMove(map, nextX, player.position.y)) player.position.x = nextX;
    const float nextY = player.position.y + player.direction.y * distance;
    if (canMove(map, player.position.x, nextY)) player.position.y = nextY;
}

void updatePlayer(Player& player, const Map& map, const InputState& input,
                  float deltaSeconds) {
    const float frameSeconds = clampDeltaSeconds(deltaSeconds);
    const int rotationDirection = static_cast<int>(input.rotateRight) -
                                  static_cast<int>(input.rotateLeft);

    if (rotationDirection != 0) {
        rotatePlayer(player,
                     rotationDirection * config::ROTATION_SPEED * frameSeconds);
    }
    if (input.forward)
        moveForward(player, map, config::MOVE_SPEED * frameSeconds);
}
