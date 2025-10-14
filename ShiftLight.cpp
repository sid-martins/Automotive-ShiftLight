#include "ShiftLight.h"

#define COR_1 0x00FF00 // Verde
#define COR_2 0xFFFF00 // Amarelo
#define COR_3 0xFF0000 // Vermelho

ShiftLight::ShiftLight(uint8_t pin, uint8_t numPixels, uint16_t maxRPM, uint16_t shiftRPM)
  : pixels(numPixels, pin, NEO_GRB + NEO_KHZ800) {
  _maxRPM = maxRPM;
  _shiftRPM = shiftRPM;
  _ledsToLight = 0;
  _lastLedsToLight = 0;
  _lastBlinkTime = 0;
  _blinkState = false;
}

void ShiftLight::begin() {
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void ShiftLight::setBrightness(uint8_t b) {
  pixels.setBrightness(b);
}

void ShiftLight::update(uint16_t rpm) {
  _ledsToLight = map(rpm, 0, _maxRPM, 0, pixels.numPixels());

  if (rpm >= _shiftRPM) {
    unsigned long now = millis();
    if (now - _lastBlinkTime >= 100) {
      _lastBlinkTime = now;
      _blinkState = !_blinkState;

      if (_blinkState) pixels.fill(COR_3, 0, pixels.numPixels());
      else pixels.clear();

      pixels.show();
    }
  } else if (_ledsToLight != _lastLedsToLight) {
    showRPM(_ledsToLight);
    _lastLedsToLight = _ledsToLight;
  }
}

uint32_t ShiftLight::getColorForLED(uint8_t index) {
  if (index < 4) return COR_1;
  else if (index < 10) return COR_2;
  else return COR_3;
}

void ShiftLight::showRPM(uint8_t leds) {
  pixels.clear();

  switch (leds) {
    case 16: pixels.setPixelColor(15, getColorForLED(15));
          // fall through
    case 15: pixels.setPixelColor(14, getColorForLED(14));
            // fall through
    case 14: pixels.setPixelColor(13, getColorForLED(13));
            // fall through
    case 13: pixels.setPixelColor(12, getColorForLED(12));
            // fall through
    case 12: pixels.setPixelColor(11, getColorForLED(11));
            // fall through
    case 11: pixels.setPixelColor(10, getColorForLED(10));
            // fall through
    case 10: pixels.setPixelColor(9, getColorForLED(9));
            // fall through
    case 9:  pixels.setPixelColor(8, getColorForLED(8));
            // fall through
    case 8:  pixels.setPixelColor(7, getColorForLED(7));
            // fall through
    case 7:  pixels.setPixelColor(6, getColorForLED(6));
            // fall through
    case 6:  pixels.setPixelColor(5, getColorForLED(5));
            // fall through
    case 5:  pixels.setPixelColor(4, getColorForLED(4));
            // fall through
    case 4:  pixels.setPixelColor(3, getColorForLED(3));
            // fall through
    case 3:  pixels.setPixelColor(2, getColorForLED(2));
            // fall through
    case 2:  pixels.setPixelColor(1, getColorForLED(1));
            // fall through
    case 1:  pixels.setPixelColor(0, getColorForLED(0));
    case 0:  break;
  }

  pixels.show();
}

void ShiftLight::clear() {
  pixels.clear();
  pixels.show();
}
