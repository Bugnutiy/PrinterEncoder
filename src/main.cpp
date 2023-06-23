#include <Arduino.h>
#define DEBUG 115200 // скорость монитора порта. Или 0 для отключения

#define PIN_ENC_A 2          // вход А
#define PIN_ENC_B 3          // вход Б
#define PINMODE INPUT_PULLUP // Какой у нас пинмод

#if DEBUG
#define DD(x) Serial.print(x)
#define DDD(x) Serial.println(x)
#else
#define DD(x)
#define DDD(x)
#endif
#include "encoder.h"

Encoder encoder; //класс энкодера
void setup()
{
#if DEBUG
    Serial.begin(DEBUG);
#endif
}

void loop()
{
}