#include "ShiftLight.h"

ShiftLight::ShiftLight(uint8_t pin, uint16_t numPixels, uint16_t maxRPM, uint16_t shiftRPM)
    : pixels(numPixels, pin, NEO_GRB + NEO_KHZ800), _maxRPM(maxRPM), _shiftRPM(shiftRPM)
{
    _shiftlight_timer = 0;
    _leds_same_since = 0;
}

void ShiftLight::begin() {
    pixels.begin();
    pixels.clear();
    pixels.show();
}

void ShiftLight::setBrightness(uint8_t b) {
    pixels.setBrightness(b);
}

void ShiftLight::update(Dados &dados) {
    uint8_t maxLeds = (uint8_t) (pixels.numPixels() < 16 ? pixels.numPixels() : 16);
    long mapped = map((long)dados.rpm, 0L, (long)_maxRPM, 0L, 16L);
    if (mapped < 0) mapped = 0;
    if (mapped > 16) mapped = 16;
    dados.leds_to_light = (uint8_t)mapped;

    unsigned long currentMillis = millis();

    if (dados.rpm >= _shiftRPM) {
        if (currentMillis - _shiftlight_timer >= 100UL) {
            _shiftlight_timer = currentMillis;
            uint32_t redColor = pixels.Color(255, 0, 0);
            if (pixels.getPixelColor(0) == redColor) {
                pixels.clear();
            } else {
                pixels.fill(redColor, 0, pixels.numPixels());
            }
            pixels.show();
        }
        _leds_same_since = currentMillis;
        return;
    }

    if (dados.leds_to_light != dados.last_leds_to_light) {
        _showBySwitch(dados.leds_to_light);
        pixels.show();
        dados.last_leds_to_light = dados.leds_to_light;
        _leds_same_since = currentMillis;
    } else {
        if (_leds_same_since == 0) _leds_same_since = currentMillis;
        if (currentMillis - _leds_same_since >= _leds_timeout_ms) {
            pixels.clear();
            pixels.show();
            _leds_same_since = currentMillis;
        }
    }
}

void ShiftLight::clearAll() {
    pixels.clear();
    pixels.show();
}

void ShiftLight::_showBySwitch(uint8_t leds) {
    pixels.clear();
    uint8_t total = pixels.numPixels();
    if (leds > total) leds = total;

    switch (leds) {
        case 16: pixels.setPixelColor(15, COR_3); // fall through
        case 15: pixels.setPixelColor(14, COR_3); // fall through
        case 14: pixels.setPixelColor(13, COR_3); // fall through
        case 13: pixels.setPixelColor(12, COR_3); // fall through
        case 12: pixels.setPixelColor(11, COR_3); // fall through
        case 11: pixels.setPixelColor(10, COR_3); // fall through
        case 10: pixels.setPixelColor(9, COR_2);  // fall through
        case 9:  pixels.setPixelColor(8, COR_2);  // fall through
        case 8:  pixels.setPixelColor(7, COR_2);  // fall through
        case 7:  pixels.setPixelColor(6, COR_2);  // fall through
        case 6:  pixels.setPixelColor(5, COR_2);  // fall through
        case 5:  pixels.setPixelColor(4, COR_2);  // fall through
        case 4:  pixels.setPixelColor(3, COR_1);  // fall through
        case 3:  pixels.setPixelColor(2, COR_1);  // fall through
        case 2:  pixels.setPixelColor(1, COR_1);  // fall through
        case 1:  pixels.setPixelColor(0, COR_1);  break;
        case 0:
        default: break;
    }
}
