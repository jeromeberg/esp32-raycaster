#include "input.h"

#include <Arduino.h>

#include "config.h"

void Input::begin() {
    pinMode(config::FORWARD_BUTTON_PIN, INPUT_PULLUP);
    pinMode(config::LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(config::RIGHT_BUTTON_PIN, INPUT_PULLUP);
}

InputState Input::read() const {
    return {
        digitalRead(config::FORWARD_BUTTON_PIN) == LOW,
        digitalRead(config::LEFT_BUTTON_PIN) == LOW,
        digitalRead(config::RIGHT_BUTTON_PIN) == LOW,
    };
}
