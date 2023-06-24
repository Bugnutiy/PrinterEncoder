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
#define ENC_ALG 3                // Алгоритм чтения энкодера (1 - 3)
#define ENC_TYPE 1               // Настройка для алгоритма №1 (1 или 2)
#define ENC_INVERSE false        // инвертировать
// #define ENC_READ 1               // Старый читатель энкодера

#define STEP_DIVIDER 17 // Сколько шагов энкодера на 1 шаг двигателя

// Кнопки
#define PIN_BTN_L 12               // Пин кнопки влево
#define PINMODE_BTN_L INPUT_PULLUP // Пинмод кнопки влево
#define BTN_L_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_R 13               // Пин кнопки вправо
#define PINMODE_BTN_R INPUT_PULLUP // Пинмод кнопки вправо
#define BTN_R_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_START A0               // Пин кнопки старт
#define PINMODE_BTN_START INPUT_PULLUP // Пинмод кнопки вверх
#define BTN_START_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_U A1               // Пин кнопки вверх
#define PINMODE_BTN_U INPUT_PULLUP // Пинмод кнопки вверх
#define BTN_U_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_D A2               // Пин кнопки вниз
#define PINMODE_BTN_D INPUT_PULLUP // Пинмод кнопки вниз
#define BTN_D_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define FILTER_BTN_LR 20    // Фильтр мс X
#define FILTER_BTN_UD 20    // Фильтр мс Y
#define FILTER_BTN_START 20 // Фильтр мс START

#define BTN_HOLD_LR 1500    // Задержка перед автоматическим движением X (0 для отключения)
#define BTN_HOLD_UD 1500    // Задержка перед автоматическим движением Y (0 для отключения)
#define BTN_HOLD_START 1500 // Кнопки start (0 для отключения)

// Настройки двигателей
#define PIN_STEP_LR 4   // Пин STEP горизонтального двигателя
#define PIN_DIR_LR 5    // Пин dir горизонтального двигателя
#define PIN_ENABLE_LR 6 // Пин enable, возможно он называется set, горизонтального двигателя

#define PIN_STEP_UD 4   // Пин STEP вертикального двигателя
#define PIN_DIR_UD 5    // Пин dir вертикального двигателя
#define PIN_ENABLE_UD 6 // Пин enable, возможно он называется set, вертикального двигателя

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
#define PAPER_DAT_TRUE HIGH   // Бумага есть = HIGH или LOW
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
#include "GyverButton.h"
#include "encoder.h"
#include "endbuttons.h"
#include "functions.h"

Encoder encoder; // класс энкодера

endBtn endLeft(PIN_END_LEFT, PINMODE_END_LR, END_TRUE_LR); // концевики
endBtn endRight(PIN_END_RIGHT, PINMODE_END_LR, END_TRUE_LR);
endBtn endUp(PIN_END_RIGHT, PINMODE_END_LR, END_TRUE_LR);
endBtn endDown(PIN_END_RIGHT, PINMODE_END_LR, END_TRUE_LR);

GButton btnLeft(PIN_BTN_L, HIGH_PULL, BTN_L_TRUE);
GButton btnRight(PIN_BTN_R, HIGH_PULL, BTN_R_TRUE);
GButton btnStart(PIN_BTN_START, HIGH_PULL, BTN_START_TRUE);
GButton btnUp(PIN_BTN_U, HIGH_PULL, BTN_U_TRUE);
GButton btnDown(PIN_BTN_D, HIGH_PULL, BTN_D_TRUE);

GStepper2<STEPPER2WIRE> stepperLR(0, PIN_STEP_LR, PIN_DIR_LR, PIN_ENABLE_LR); // Шаговый двигатель X
GStepper2<STEPPER2WIRE> stepperUD(0, PIN_STEP_UD, PIN_DIR_UD, PIN_ENABLE_UD); // Шаговый двигатель Y

void setup()
{
#if DEBUG
    Serial.begin(DEBUG);
#endif
    btnLeft.setTimeout(BTN_HOLD_LR);
    btnLeft.setTickMode(AUTO);
    pinMode(PIN_BTN_L, PINMODE_BTN_L);

    btnRight.setTimeout(BTN_HOLD_LR);
    btnRight.setTickMode(AUTO);
    pinMode(PIN_BTN_R, PINMODE_BTN_R);

    btnStart.setTimeout(BTN_HOLD_START);
    btnStart.setTickMode(AUTO);
    pinMode(PIN_BTN_START, PINMODE_BTN_START);

    btnUp.setTimeout(BTN_HOLD_UD);
    btnUp.setTickMode(AUTO);
    pinMode(PIN_BTN_U, PINMODE_BTN_U);

    btnDown.setTimeout(BTN_HOLD_UD);
    btnDown.setTickMode(AUTO);
    pinMode(PIN_BTN_D, PINMODE_BTN_D);

    pinMode(PIN_PAPER_DAT, OUTPUT);

    stepperLR.brake();
    stepperUD.brake();
}
/**
 * @brief 0 - режим StandBy;
 * @brief 1 - движение на домашний концевик с лазерной коррекцией;
 * @brief 2 - движение к стартовой позиции;
 * @brief 3 - Ожидание начала печати;
 * @brief 4 - начало печати (датчики бумаги, ждём когда принтер перестанет сходить с ума);
 * @brief 5 - печать (работа от энкодера);
 * @brief 6 - конец печати;
 * @brief 7 - Выдвижение каретки;
 *
 * @brief 8 - Работа по кнопке;
 *
 */
uint8_t worker = 5;
void loop()
{
    switch (worker)
    {
    case 0: // 0 - режим StandBy;
        stepperLR.brake();
        stepperUD.brake();

        break;
    case 1: // 1 - движение на домашний концевик с лазерной коррекцией; (Влево наверно)
        /* code */
        break;
    case 2: // 2 - движение к стартовой позиции;
        /* code */
        break;
    case 3: // 3 - Ожидание начала печати;
        /* code */
        break;
    case 4: // 4 - начало печати (датчики бумаги, ждём когда принтер перестанет сходить с ума);
        /* coe */
        break;
    case 5: // 5 - печать (работа от энкодера);
        stepperLR.setTarget(encoder.getEncPos() / STEP_DIVIDER);
        break;
    case 6: // 6 - конец печати;
        /* code */
        break;
    case 7: // 7 - Выдвижение каретки;
        /* code */
        break;
    case 8: // от кнопки
        /* code */
        break;
    default:

        break;
    }

    if (endRight.state())
    {
        if (stepperLR.dir > 0 && stepperLR.getStatus())
        {
            stepperLR.brake();
            stepperLR.reset();
            encoder.resetPos();
        }
    }

    if (endLeft.state())
    {
        if (stepperLR.dir < 0 && stepperLR.getStatus())
        {
            stepperLR.brake();
            stepperLR.reset();
            encoder.resetPos();
            switch (worker)
            {
            case 8:
                worker = 0;
                break;

            default:
                worker = 0;
                break;
            }
        }
    }

    DD("STEP STATUS: ");
    DDD(stepperLR.getStatus());
    DD("STEP DIR: ");
    DDD(stepperLR.dir);
    // stepperLR.tick();
    stepperUD.tick();
    delay(300);
}