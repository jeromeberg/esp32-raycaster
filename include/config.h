#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

namespace config {
constexpr std::uint8_t SCREEN_WIDTH = 128;
constexpr std::uint8_t SCREEN_HEIGHT = 64;

constexpr std::uint8_t OLED_ADDRESS = 0x3C;
constexpr std::uint8_t OLED_SDA_PIN = 21;
constexpr std::uint8_t OLED_SCL_PIN = 22;
constexpr std::uint32_t OLED_BUS_FREQUENCY = 400000;

constexpr std::uint8_t FORWARD_BUTTON_PIN = 25;
constexpr std::uint8_t LEFT_BUTTON_PIN = 32;
constexpr std::uint8_t RIGHT_BUTTON_PIN = 33;

constexpr float CAMERA_PLANE_LENGTH = 0.66f;
constexpr float MOVE_SPEED = 2.0f;
constexpr float ROTATION_SPEED = 1.8f;
constexpr float COLLISION_RADIUS = 0.20f;
constexpr float MAX_FRAME_SECONDS = 0.05f;
}  // namespace config

#endif
