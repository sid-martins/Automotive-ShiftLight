#include "ShiftLight.h"

#define COR_1 0x00FF00 // Verde
#define COR_2 0xFFFF00 // Amarelo
#define COR_3 0xFF0000 // Vermelho

ShiftLight::ShiftLight(uint8_t pin, uint8_t numPixels, uint16_t maxRPM, uint16_t shiftRPM)
  : pixels(numPixels, pin, NEO_GRB + NEO_KHZ800) {
  _maxRPM = maxRPM;
  _shiftRPM = shiftRPM;
  _ledsToLight = 0;
  _lastLedsToLight = 255;  // força atualização no primeiro loop
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
  // Garante que o valor fique dentro dos limites
  rpm = constrain(rpm, 0, _maxRPM);
  uint8_t num = pixels.numPixels();

  // Calcula LEDs acesos proporcionalmente ao RPM
  _ledsToLight = map(rpm, 0, _maxRPM, 0, num);
  if (_ledsToLight > num) _ledsToLight = num;

  // Modo piscante (shift)
  if (rpm >= _shiftRPM) {
    unsigned long now = millis();
    if (now - _lastBlinkTime >= 100) {  // pisca a cada 100ms
      _lastBlinkTime = now;
      _blinkState = !_blinkState;

      if (_blinkState) pixels.fill(COR_3, 0, num);
      else pixels.clear();

      pixels.show();
    }
  } 
  else {
    // Saiu do modo shift, garante LEDs acesos normalmente
    if (_blinkState) {
      _blinkState = false;
      pixels.clear();
    }

    // Atualiza LEDs apenas se houve mudança
    if (_ledsToLight != _lastLedsToLight) {
      showRPM(_ledsToLight);
      _lastLedsToLight = _ledsToLight;
    }
  }
}

uint32_t ShiftLight::getColorForLED(uint8_t index) {
  // Divisão em zonas
  if (index < 4) return COR_1;      // primeiros LEDs: verde
  else if (index < 10) return COR_2; // meio: amarelo
  else return COR_3;                // final: vermelho
}

void ShiftLight::showRPM(uint8_t leds) {
  pixels.clear();

  // Garante que não ultrapasse o total de LEDs
  if (leds > pixels.numPixels()) leds = pixels.numPixels();

  // Acende LEDs de 0 até leds-1
  for (uint8_t i = 0; i < leds; i++) {
    pixels.setPixelColor(i, getColorForLED(i));
  }

  pixels.show();
}

void ShiftLight::clear() {
  pixels.clear();
  pixels.show();
}
