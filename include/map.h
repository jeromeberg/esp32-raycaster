#ifndef MAP_H
#define MAP_H

#include <cstdint>

class Map {
 public:
    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;

    bool isWall(int x, int y) const;

 private:
    static const std::uint8_t CELLS[HEIGHT][WIDTH];
};

#endif
