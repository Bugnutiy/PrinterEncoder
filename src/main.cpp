#include <Arduino.h>
#define PIN_ENC_A 2 // вход А
#define PIN_ENC_B 3 // вход Б

#define DEBUG 9600 // скорость монитора порта. Или 0 для отключения
#if DEBUG
#define DD(x) Serial.print(x)
#define DDD(x) Serial.println(x)
#else
#define DD(x)
#define DDD(x)
#endif
#include "encoder.h"

Encoder encoder;
void setup()
{
#if DEBUG
    Serial.begin(DEBUG);
#endif
attachInterrupt(0,*&encoder.encInterrupt(),CHANGE);
}

void loop()
{
}