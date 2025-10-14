#include <ShiftLight.h>

#define PIN_LED 8 // Pino do MCU - MCU Pin
#define NUM_LEDS 16 // Quantidade de LEDS - LEDS quantity
#define MAX_RPM 6500 // RPM maximo - Max RPM
#define SHIFT_RPM 6300 // Faixa de RPM para troca de marcha - RPM value to shift

ShiftLight shift(PIN_LED, NUM_LEDS, MAX_RPM, SHIFT_RPM);

void setup() {
  Serial.begin(115200);
  shift.begin();

  // Ajusta o brilho global para 50% - Ajust brightness in 50%
  shift.setBrightness(128); // 0-255
}

void loop() {
  static uint16_t rpm = 0;
  rpm += 100;
  if (rpm > 7000) rpm = 0;

  shift.update(rpm); // Função que atualiza os leds com base no RPM - Funtion to update LEDS for RPM
  delay(200);
}
