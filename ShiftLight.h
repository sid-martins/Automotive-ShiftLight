#ifndef SHIFTLIGHT_H
#define SHIFTLIGHT_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class ShiftLight {
  public:
    ShiftLight(uint8_t pin, uint8_t numPixels, uint16_t maxRPM, uint16_t shiftRPM);
    void begin();
    void update(uint16_t rpm);
    void clear();
    void setBrightness(uint8_t b);   // <<< NOVA FUNÇÃO

  private:
    Adafruit_NeoPixel pixels;
    uint16_t _maxRPM;
    uint16_t _shiftRPM;
    uint8_t _ledsToLight;
    uint8_t _lastLedsToLight;
    unsigned long _lastBlinkTime;
    bool _blinkState;

    uint32_t getColorForLED(uint8_t index);
    void showRPM(uint8_t leds);
};

#endif
