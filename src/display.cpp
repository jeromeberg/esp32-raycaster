#include "display.h"

#include <U8g2lib.h>

#include "config.h"

namespace {
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE,
                                         config::OLED_SCL_PIN,
                                         config::OLED_SDA_PIN);
}

bool OledDisplay::begin() {
    oled.setI2CAddress(config::OLED_ADDRESS << 1);
    oled.setBusClock(config::OLED_BUS_FREQUENCY);
    return oled.begin();
}

void OledDisplay::clear() { oled.clearBuffer(); }

void OledDisplay::drawVerticalLine(std::uint8_t x, std::uint8_t y,
                                   std::uint8_t height) {
    oled.drawVLine(x, y, height);
}

void OledDisplay::present() { oled.sendBuffer(); }
