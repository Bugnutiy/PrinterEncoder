#include <Arduino.h>
#define DEBUG 115200 // скорость монитора порта. Или 0 для отключения
// Настройка энкодера
#define PIN_ENC_A 2              // вход А
#define PIN_ENC_B 3              // вход Б
#define PINMODE_ENC INPUT_PULLUP // Какой у нас пинмод энкодера

// Настройки двигателя
#define PIN_STEP 4   // Пин STEP
#define PIN_DIR 5    // Пин dir
#define PIN_ENABLE 6 // Пин enable, возможно он называется set

// Настройки концевиков и лазера
#define PIN_END_LEFT 7
#define PIN_END_RIGHT 8
#define PINMODE_END_LR INPUT_PULLUP
#define END_TRUE_LR LOW
#define END_HOME_LR 0 //начальная позиция слева (0), начальная позиция справа (1)

#define LASER
#define PIN_END_UP 9
#define PIN_END_DOWN 10
#define PINMODE_END_UD INPUT_PULLUP
#define END_TRUE_UD LOW
#define END_HOME_UD 1 //начальная позиция снизу (0), начальная позиция сверху (1)

///////////////Ниже код, настроек нет.

#if DEBUG
#define DD(x) Serial.print(x)
#define DDD(x) Serial.println(x)
#else
#define DD(x)
#define DDD(x)
#endif

#include "GyverStepper2.h"
#include "encoder.h"
#include "endbuttons.h"
#include "functions.h"

Encoder encoder; // класс энкодера
endBtn endLeft(PIN_END_LEFT, PINMODE_END_LR, END_TRUE_LR);
endBtn endRight(PIN_END_RIGHT, PINMODE_END_LR, END_TRUE_LR);
endBtn endUp(PIN_END_RIGHT, PINMODE_END_LR, END_TRUE_LR);
endBtn endDown(PIN_END_RIGHT, PINMODE_END_LR, END_TRUE_LR);
GStepper2<STEPPER2WIRE> stepper(0, PIN_STEP, PIN_DIR, PIN_ENABLE); // Шаговый двигатель

void setup()
{
#if DEBUG
    Serial.begin(DEBUG);
#endif
}

void loop()
{
}