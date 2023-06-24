/**
 * @file main.cpp
 * @author Bugnutiy ()
 * @brief Ребилд кода для управления принтером
 * @version 0.1
 * @date 2023-06-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#define DEBUG 115200 // скорость монитора порта. Или 0 для отключения
// Настройка энкодера
#define PIN_ENC_A 2              // вход А
#define PIN_ENC_B 3              // вход Б
#define PINMODE_ENC INPUT_PULLUP // Пинмод энкодера

#define ENC_ALG 3       // Алгоритм чтения энкодера (1 - 3)
#define ENC_TYPE 1      // Настройка для алгоритма №1 (1 или 2)
#define STEP_DIVIDER 17 // Сколько шагов энкодера на 1 шаг двигателя
// Кнопки
#define PIN_BTN_L 12               // Пин кнопки влево
#define PINMODE_BTN_L INPUT_PULLUP // Пинмод кнопки влево
#define BTN_L_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_R 13               // Пин кнопки вправо
#define PINMODE_BTN_R INPUT_PULLUP // Пинмод кнопки вправо
#define BTN_R_TRUE LOW             // Логика срабатывания (LOW/HIGH)

// Настройки двигателей
#define PIN_STEP_LR 4          // Пин STEP горизонтального двигателя
#define PIN_DIR_LR 5           // Пин dir горизонтального двигателя
#define PIN_ENABLE_LR 6        // Пин enable, возможно он называется set, горизонтального двигателя

#define PIN_STEP_UD 4          // Пин STEP вертикального двигателя
#define PIN_DIR_UD 5           // Пин dir вертикального двигателя
#define PIN_ENABLE_UD 6        // Пин enable, возможно он называется set, вертикального двигателя

// Настройки концевиков и лазера
#define PIN_END_LEFT 7              // левый концевик
#define PIN_END_RIGHT 8             // правый концевик
#define PINMODE_END_LR INPUT_PULLUP // Пинмод концевиков
#define END_TRUE_LR LOW             // Логика срабатывания (LOW/HIGH)
#define END_HOME_LR 0               // начальная позиция слева (0), начальная позиция справа (1)
#define FILTER_END_LR 20            // Фильтр мс

#define PIN_LASER 9     // Пин лазера
#define LASER_TRUE LOW  // Логика срабатывания (LOW/HIGH)
#define FILTER_LASER 20 // Фильтр мс

#define PIN_END_UP 10               // Верхний концевик
#define PIN_END_DOWN 11             // Нижний концевик
#define PINMODE_END_UD INPUT_PULLUP // Пинмод концевиков
#define END_TRUE_UD LOW             // Логика срабатывания (LOW/HIGH)
#define END_HOME_UD 1               // начальная позиция снизу (0), начальная позиция сверху (1)
#define FILTER_END_UD 20            // Фильтр мс

// лотки, датчики бумаги, и т.д.
#define PIN_PAPER_DAT 12      // Пин датчика бумаги
#define PAPER_TIME_START 1700 // Тайминг датчика бумаги после начала печати
#define PAPER_TIME_END 0      // Тайминг датчика бумаги после конца печати

// настройки движений и таймингов
#define MOTOR_X_ENC_SPEED 200 // Скорость мотора в шагах/секунду. Чем выше число, тем быстрее двигатель
#define MOTOR_Y_ENC_SPEED 200 // Скорость мотора в шагах/секунду. Чем выше число, тем быстрее двигатель
#define MOTOR_L_BUTTON_SPEED 100 // Скорость движения по кнопке влево
#define MOTOR_R_BUTTON_SPEED 100 // Скорость движения по кнопке вправо
#define MOTOR_U_BUTTON_SPEED 100 // Скорость движения по кнопке вверх
#define MOTOR_D_BUTTON_SPEED 100 // Скорость движения по кнопке вниз
#define MOTOR_L_AUTO_SPEED 50
#define MOTOR_R_AUTO_SPEED 50
#define MOTOR_U_AUTO_SPEED 50
#define MOTOR_D_AUTO_SPEED 50
#define GS_NO_ACCEL
////////////////////////////////////////////////////////*Настройки кончились*//////////////////////////////////////////////////////////////

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
GStepper2<STEPPER2WIRE> stepperLR(0, PIN_STEP_LR, PIN_DIR_LR, PIN_ENABLE_LR); // Шаговый двигатель
GStepper2<STEPPER2WIRE> stepperUD(0, PIN_STEP_UD, PIN_DIR_UD, PIN_ENABLE_UD); // Шаговый двигатель

void setup()
{
#if DEBUG
    Serial.begin(DEBUG);
#endif
}

void loop()
{
}