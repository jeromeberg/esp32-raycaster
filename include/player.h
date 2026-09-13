#ifndef PLAYER_H
#define PLAYER_H

struct Vector2 {
    float x;
    float y;
};

struct Player {
    Vector2 position;
    Vector2 direction;
    Vector2 cameraPlane;
};

#endif
