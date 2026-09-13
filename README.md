# esp32-raycaster

A tiny raycasting engine inspired by [cub3d](https://github.com/jeromeberg/cub3d) and rewritten in C++ for ESP32 and a 128x64 monochrome OLED.

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=flat&logo=espressif&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=cplusplus&logoColor=white)

![demo](demo.gif)

## Hardware

- NodeMCU-32S
- SSD1306 128x64 I2C OLED
- 3 buttons

| Component      | Pin     |
|----------------|---------|
| OLED SDA       | GPIO 21 |
| OLED SCL       | GPIO 22 |
| Forward button | GPIO 25 |
| Left button    | GPIO 32 |
| Right button   | GPIO 33 |

## Instructions

### Build

```sh
pio run -e nodemcu-32s
```

### Upload

```sh
pio run -e nodemcu-32s -t upload
```

### Tests

```sh
pio test -e native
```