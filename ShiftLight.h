#ifndef SHIFTLIGHT_H
#define SHIFTLIGHT_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

struct Dados {
  uint16_t rpm;
  uint8_t leds_to_light;
  uint8_t last_leds_to_light;
};

class ShiftLight {
public:
    // Construtor com pin, número de LEDs, maxRPM e shiftRPM
    ShiftLight(uint8_t pin, uint16_t numPixels, uint16_t maxRPM, uint16_t shiftRPM);

    void begin();
    void setBrightness(uint8_t b);
    void update(Dados &dados);
    void clearAll();

private:
    Adafruit_NeoPixel pixels;
    uint16_t _maxRPM;
    uint16_t _shiftRPM;

    unsigned long _shiftlight_timer;
    unsigned long _leds_same_since;
    const unsigned long _leds_timeout_ms = 5000UL; // 5s para apagar por timeout

    // cores internas da biblioteca
    const uint32_t COR_1 = 0x00FF00; // verde
    const uint32_t COR_2 = 0xFFFF00; // amarelo
    const uint32_t COR_3 = 0xFF0000; // vermelho

    void _showBySwitch(uint8_t leds);
};

#endif
