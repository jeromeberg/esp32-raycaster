#include "map.h"

const std::uint8_t Map::CELLS[Map::HEIGHT][Map::WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1},
};

bool Map::isWall(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return true;
    return CELLS[y][x] != 0;
}
