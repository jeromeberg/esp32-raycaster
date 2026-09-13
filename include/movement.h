#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "input.h"
#include "map.h"
#include "player.h"

void updatePlayer(Player& player, const Map& map, const InputState& input,
                  float deltaSeconds);

#endif
