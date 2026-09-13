#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>

class Display {
 public:
    virtual ~Display() = default;
    virtual bool begin() = 0;
    virtual void clear() = 0;
    virtual void drawVerticalLine(std::uint8_t x, std::uint8_t y,
                                  std::uint8_t height) = 0;
    virtual void present() = 0;
};

class OledDisplay final : public Display {
 public:
    bool begin() override;

    void clear() override;

    void drawVerticalLine(std::uint8_t x, std::uint8_t y,
                          std::uint8_t height) override;

    void present() override;
};

#endif
