#include <Arduino.h>

#include "config.h"
#include "display.h"
#include "input.h"
#include "map.h"
#include "movement.h"
#include "player.h"
#include "renderer.h"

namespace {
Map gameMap;
Player player = {
    {1.5f, 1.5f},
    {1.0f, 0.0f},
    {0.0f, config::CAMERA_PLANE_LENGTH},
};
Input input;
OledDisplay display;
std::uint32_t previousFrameMillis = 0;

float calculateFrameSeconds(std::uint32_t currentMillis) {
    const std::uint32_t elapsedMillis = currentMillis - previousFrameMillis;
    const float elapsedSeconds = static_cast<float>(elapsedMillis) / 1000.0f;
    if (elapsedSeconds > config::MAX_FRAME_SECONDS)
        return config::MAX_FRAME_SECONDS;
    return elapsedSeconds;
}
}  // namespace

void setup() {
    Serial.begin(115200);
    input.begin();
    if (!display.begin()) Serial.println("SSD1306 initialization failed");
    previousFrameMillis = millis();
    renderFrame(gameMap, player, display);
}

void loop() {
    const std::uint32_t currentMillis = millis();
    const float deltaSeconds = calculateFrameSeconds(currentMillis);
    previousFrameMillis = currentMillis;
    updatePlayer(player, gameMap, input.read(), deltaSeconds);
    renderFrame(gameMap, player, display);
}
