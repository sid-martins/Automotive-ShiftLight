# ShiftLight

Biblioteca para criar **shiftlights** usando LEDs WS2812B (NeoPixels) com suporte a brilho global e controle de RPM.
Desenvolvida para Arduino, usando **switch/case** para performance máxima sem loops, ideal para projetos de automação veicular, karts ou simuladores.

---

## Funcionalidades

* Atualização de LEDs sem loops (`for`/`while`) usando `switch/case`.
* Controle de brilho global (`setBrightness`).
* Suporte a limites de RPM e alerta de **shift RPM** (pisca em vermelho quando atingido).
* Compacta e fácil de integrar em outros projetos.

---

## Estrutura da biblioteca

```
ShiftLight/
├── ShiftLight.h
├── ShiftLight.cpp
└── library.properties
```

---

## Instalação

### 1. Via arquivo ZIP

1. Compacte a pasta `ShiftLight` em um arquivo `.zip` (por exemplo: `ShiftLight.zip`).
2. Abra o Arduino IDE.
3. Vá em **Sketch → Include Library → Add .ZIP Library…**
4. Selecione o arquivo `.zip`.
5. A biblioteca estará pronta para uso e você poderá incluir em seus sketches com:

```cpp
#include <ShiftLight.h>
```

### 2. Manual

1. Copie a pasta `ShiftLight` para a pasta de bibliotecas do Arduino, normalmente em:

   * **Windows:** `Documents/Arduino/libraries/`
   * **Mac:** `Documents/Arduino/libraries/`
   * **Linux:** `~/Arduino/libraries/`
2. Reinicie o Arduino IDE.
3. Inclua no sketch:

```cpp
#include <ShiftLight.h>
```

---

## Como usar

### Exemplo básico

```cpp
#include <ShiftLight.h>

#define PIN_LED 8       // Pino do MCU
#define NUM_LEDS 16     // Quantidade de LEDs
#define MAX_RPM 6500    // RPM máximo
#define SHIFT_RPM 6300  // Faixa de RPM para troca de marcha

ShiftLight shift(PIN_LED, NUM_LEDS, MAX_RPM, SHIFT_RPM);

void setup() {
  Serial.begin(115200);
  shift.begin();

  // Ajusta o brilho global para 50%
  shift.setBrightness(128); // 0-255
}

void loop() {
  static uint16_t rpm = 0;
  rpm += 100;
  if (rpm > 7000) rpm = 0;

  // Atualiza os LEDs com base no RPM
  shift.update(rpm);
  delay(200);
}
```

---

## Funções principais

| Função                     | Descrição                                       |
| -------------------------- | ----------------------------------------------- |
| `begin()`                  | Inicializa a biblioteca e apaga os LEDs.        |
| `update(uint16_t rpm)`     | Atualiza os LEDs de acordo com o RPM informado. |
| `clear()`                  | Apaga todos os LEDs.                            |
| `setBrightness(uint8_t b)` | Define o brilho global (0 a 255).               |

---

## Hardware necessário

* LEDs WS2812B (NeoPixels)
* Resistor de 330Ω no pino de dados
* Capacitor 1000 µF entre VCC e GND para estabilidade
* Fonte de alimentação adequada (não alimente muitos LEDs direto do Arduino)
* Arduino compatível (Uno, Nano, Pro Micro, etc.)

---

## Observações

* Para barras maiores de LEDs, o tempo de atualização é limitado pelo `pixels.show()`, não pelo código `switch/case`.
* O consumo de energia diminui ao reduzir o brilho com `setBrightness()`.
* O ShiftLight pisca em vermelho automaticamente quando o RPM atinge o valor de **shift RPM**.
* Você pode simular RPM no loop ou integrar com sinais de telemetria real via CAN ou sensores analógicos.

---

## Exemplo avançado (RPM vindo do CAN)

```cpp
#include <ShiftLight.h>
#include <ACAN2515.h>

ShiftLight shift(8, 16, 6500, 6300);
ACAN2515 can(10, SPI, 2); // Pinos CS e INT

void setup() {
  Serial.begin(115200);
  shift.begin();
  shift.setBrightness(128);

  ACAN2515Settings settings(8E6, 500000);
  settings.mRequestedMode = ACAN2515Settings::NormalMode;

  uint16_t errorCode = can.begin(settings, [] { can.isr(); });
  if (errorCode != 0) {
    Serial.print("Erro na configuração CAN: 0x");
    Serial.println(errorCode, HEX);
    while (1);
  }
}

void loop() {
  CANMessage msg;
  if (can.receive(msg) && msg.id == 0x100) { // ID RPM
    uint16_t rpm = (msg.data[0] << 8) | msg.data[1];
    shift.update(rpm);
  }
}
```

---

## Licença

MIT License
Desenvolvido por **Sidnei Martins**
