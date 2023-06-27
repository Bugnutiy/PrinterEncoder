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

// Вертикальные кнопки по клику, горизонтальныые по Холду, Старт по клику.

// #define DEBUG 115200 // скорость монитора порта. Или 0 для отключения

// Настройка энкодера
#define PIN_ENC_A 2              // вход А
#define PIN_ENC_B 3              // вход Б
#define PINMODE_ENC INPUT_PULLUP // Пинмод энкодера
#define ENC_ALG 1                // Алгоритм чтения энкодера (1 - 3)
#define ENC_TYPE 2               // Настройка для алгоритма №1 (1 или 2)
#define ENC_INVERSE true         // инвертировать
#define ENC_TRIGGER 18            // Пульсы, означающие старт печати
// #define ENC_READ 1               // Старый читатель энкодера

#define STEP_DIVIDER 8 // Сколько шагов энкодера на 1 шаг двигателя

// Кнопки
#define PIN_BTN_L 6                // Пин кнопки влево
#define PINMODE_BTN_L INPUT_PULLUP // Пинмод кнопки влево
#define BTN_L_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_R 7                // Пин кнопки вправо
#define PINMODE_BTN_R INPUT_PULLUP // Пинмод кнопки вправо
#define BTN_R_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_START 8                // Пин кнопки старт
#define PINMODE_BTN_START INPUT_PULLUP // Пинмод кнопки вверх
#define BTN_START_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_U A1               // Пин кнопки вверх
#define PINMODE_BTN_U INPUT_PULLUP // Пинмод кнопки вверх
#define BTN_U_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define PIN_BTN_D A0               // Пин кнопки вниз
#define PINMODE_BTN_D INPUT_PULLUP // Пинмод кнопки вниз
#define BTN_D_TRUE LOW             // Логика срабатывания (LOW/HIGH)

#define FILTER_BTN_LR 20    // Фильтр мс X
#define FILTER_BTN_UD 20    // Фильтр мс Y
#define FILTER_BTN_START 20 // Фильтр мс START

#define BTN_HOLD_LR 2000   // Задержка перед автоматическим движением X (0 для отключения)
#define BTN_HOLD_UD 2000   // Задержка перед автоматическим движением Y (0 для отключения)
#define BTN_HOLD_START 200 // Кнопки start (0 для отключения)

// Настройки двигателей
#define PIN_STEP_LR 13 // Пин STEP горизонтального двигателя
#define PIN_DIR_LR 12  // Пин dir горизонтального двигателя
// #define PIN_ENABLE_LR     // Пин enable, возможно он называется set, горизонтального двигателя
#define MOTOR_X_INVERT true // Инверсия двигателя

#define PIN_STEP_UD 9 // Пин STEP вертикального двигателя
#define PIN_DIR_UD 10 // Пин dir вертикального двигателя
// #define PIN_ENABLE_UD A5 // Пин enable, возможно он называется set, вертикального двигателя
#define MOTOR_Y_INVERT false // Инверсия двигателя

// Настройки концевиков и лазера
#define PIN_END_LEFT 4              // левый концевик
#define PIN_END_RIGHT 5             // правый концевик
#define PINMODE_END_LR INPUT_PULLUP // Пинмод концевиков
#define END_TRUE_LR LOW             // Логика срабатывания (LOW/HIGH)
#define FILTER_END_LR 20            // Фильтр мс

#define PIN_LASER A4               // Пин лазера
#define PINMODE_LASER INPUT_PULLUP // Пинмод лазера
#define LASER_TRUE HIGH            // Логика срабатывания (LOW/HIGH)
#define FILTER_LASER 20            // Фильтр мс
#define LASER_MIN_STEPS 500        // Минималка лазера по шагам

#define PIN_END_UP A2               // Верхний концевик
#define PIN_END_DOWN A3             // Нижний концевик
#define PINMODE_END_UD INPUT_PULLUP // Пинмод концевиков
#define END_TRUE_UD LOW             // Логика срабатывания (LOW/HIGH)
#define FILTER_END_UD 200           // Фильтр мс

#define END_HOME_LR 0 // Стартовая позиция слева (0), справа (1)
#define END_HOME_UD 1 // Стартовая позиция снизу (0), начальная позиция сверху (1)
#define HOME_POS 100  // Стартовая позиция от концевика

// лотки, датчики бумаги, и т.д.
#define PIN_PAPER_DAT 11   // Пин датчика бумаги
#define PAPER_DAT_TRUE LOW // Бумага есть = HIGH или LOW

// настройки движений и таймингов
#define MOTOR_X_ENC_SPEED 700 // Скорость мотора в шагах/секунду. Чем выше число, тем быстрее двигатель

#define MOTOR_L_BUTTON_SPEED 100  // Скорость движения по кнопке влево
#define MOTOR_R_BUTTON_SPEED 100  // Скорость движения по кнопке вправо
#define MOTOR_U_BUTTON_SPEED 3000 // Скорость движения по кнопке вверх
#define MOTOR_D_BUTTON_SPEED 3000 // Скорость движения по кнопке вниз

#define MOTOR_L_AUTO_SPEED 300
#define MOTOR_R_AUTO_SPEED 300
#define MOTOR_Y_HOME_SPEED 3000

#define MOTOR_X_HOME_SPEED 200
// Тайминги
#define PAPER_TIME_START 1100 // Тайминг датчика бумаги после начала печати
#define ENC_TIME_START 2300
#define ENC_TIME_START_SPEED 10
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

endBtn endLeft(PIN_END_LEFT, PINMODE_END_LR, END_TRUE_LR, FILTER_END_LR); // концевики
endBtn endRight(PIN_END_RIGHT, PINMODE_END_LR, END_TRUE_LR, FILTER_END_LR);
endBtn endUp(PIN_END_UP, PINMODE_END_UD, END_TRUE_LR, FILTER_END_UD);
endBtn endDown(PIN_END_DOWN, PINMODE_END_UD, END_TRUE_UD, FILTER_END_UD);

GButton laser(PIN_LASER, HIGH_PULL, LASER_TRUE);

GButton btnLeft(PIN_BTN_L, HIGH_PULL, BTN_L_TRUE);
GButton btnRight(PIN_BTN_R, HIGH_PULL, BTN_R_TRUE);
GButton btnStart(PIN_BTN_START, HIGH_PULL, BTN_START_TRUE);
GButton btnUp(PIN_BTN_U, HIGH_PULL, BTN_U_TRUE);
GButton btnDown(PIN_BTN_D, HIGH_PULL, BTN_D_TRUE);

GStepper2<STEPPER2WIRE> stepperX(0, PIN_STEP_LR, PIN_DIR_LR); // Шаговый двигатель X
GStepper2<STEPPER2WIRE> stepperY(0, PIN_STEP_UD, PIN_DIR_UD); // Шаговый двигатель Y

void setup()
{
#if DEBUG
    Serial.begin(DEBUG);
#endif
    btnLeft.setTimeout(BTN_HOLD_LR);
    btnLeft.setTickMode(AUTO);
    btnLeft.setDebounce(FILTER_BTN_LR);
    pinMode(PIN_BTN_L, PINMODE_BTN_L);

    btnRight.setTimeout(BTN_HOLD_LR);
    btnRight.setTickMode(AUTO);
    btnRight.setDebounce(FILTER_BTN_LR);
    pinMode(PIN_BTN_R, PINMODE_BTN_R);

    btnStart.setTimeout(BTN_HOLD_START);
    btnStart.setTickMode(AUTO);
    btnStart.setDebounce(FILTER_BTN_START);
    pinMode(PIN_BTN_START, PINMODE_BTN_START);

    btnUp.setTimeout(BTN_HOLD_UD);
    btnUp.setTickMode(AUTO);
    btnUp.setDebounce(FILTER_BTN_UD);
    pinMode(PIN_BTN_U, PINMODE_BTN_U);

    btnDown.setTimeout(BTN_HOLD_UD);
    btnDown.setTickMode(AUTO);
    btnDown.setDebounce(FILTER_BTN_UD);
    pinMode(PIN_BTN_D, PINMODE_BTN_D);

    laser.setTickMode(AUTO);
    laser.setDebounce(FILTER_LASER);
    pinMode(PIN_LASER, PINMODE_LASER);

    pinMode(PIN_PAPER_DAT, OUTPUT);
    digitalWrite(PIN_PAPER_DAT, !PAPER_DAT_TRUE);

    pinMode(PIN_END_RIGHT, PINMODE_END_LR);
    pinMode(PIN_END_LEFT, PINMODE_END_LR);
    pinMode(PIN_END_DOWN, PINMODE_END_UD);
    pinMode(PIN_END_UP, PINMODE_END_UD);

    stepperX.brake();
    stepperX.reverse(MOTOR_X_INVERT);
    stepperX.setMaxSpeed(MOTOR_X_ENC_SPEED);
    // stepperX.setAcceleration(1);
    stepperY.brake();
    stepperY.reverse(MOTOR_Y_INVERT);
    stepperY.setMaxSpeed(MOTOR_Y_HOME_SPEED);
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
uint8_t worker = 8;
uint8_t worker1 = 0;
bool paper_dat = 0;
unsigned long timer_paper, timer_encoder, timer_stop_encoder;
#if DEBUG
unsigned long deb_timer = 0;
#endif
void loop()
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    switch (worker)
    {
    case 0: // 0 - режим StandBy;
        // stepperX.brake();
        // stepperY.brake();
        // worker = 8;
        break;
    case 1: // 1 - движение на домашний концевик с лазерной коррекцией; (Влево наверно)
            ////////////////////////////////////////
        switch (worker1)
        {
        case 0:
            if (!laser.state() && !stepperX.getStatus() && !stepperY.getStatus())
            {
                stepperX.setSpeed(-MOTOR_X_HOME_SPEED);
            }
            if (laser.state())
            {
                stepperX.brake();
                if (laser.state() && !stepperY.getStatus())
                {
                    stepperY.setTarget(stepperY.getCurrent() - LASER_MIN_STEPS);
                }
            }
            if (endLeft.state())
            {
                worker1++;
            }
            break;
        case 1:
            stepperX.reset();
            stepperX.setMaxSpeed(MOTOR_X_HOME_SPEED);
            stepperX.setTarget(10);
            while (stepperX.getStatus())
            {
                stepperX.tick();
            }
            stepperX.brake();
            stepperX.setSpeed(-20);

            while (!endLeft.state())
            {
                stepperX.tick();
            }
            stepperX.brake();
            stepperX.reset();
            worker++;
            worker1 = 0;
            break;
        default:
            break;
        }
        /////////////////
        break;
    case 2: // 2 - движение к стартовой позиции;
        stepperX.setMaxSpeed(MOTOR_X_HOME_SPEED);
        stepperX.setTarget(HOME_POS);
        while (stepperX.getStatus())
        {
            stepperX.tick();
        }
        stepperX.setMaxSpeed(MOTOR_X_ENC_SPEED);
        worker++;
        encoder.resetPos();

        break;
    case 3: // 3 - Ожидание начала печати;
        if (encoder.getPos() > ENC_TRIGGER || encoder.getPos() < -ENC_TRIGGER)
        {
            worker++;
            timer_paper = millis() + PAPER_TIME_START;
            timer_encoder = millis() + ENC_TIME_START;
        }

        break;
    case 4: // 4 - начало печати (датчики бумаги, ждём когда принтер перестанет сходить с ума);
        if (!paper_dat && millis() > timer_paper)
        {
            DD("PAPER:");
            paper_dat = 1;
            digitalWrite(PIN_PAPER_DAT, PAPER_DAT_TRUE);
        }
        if (millis() >= timer_encoder)
        {
            encoder.resetPos();
            int speed = 10000;
            uint32_t enc_pos_temp = 0;
            timer_stop_encoder = millis() + ENC_TIME_START_SPEED;
            while (speed)
            {
                enc_pos_temp += abs(encoder.getPos());
                if (millis() >= timer_stop_encoder)
                {
                    timer_stop_encoder = millis() + ENC_TIME_START_SPEED;
                    speed = enc_pos_temp;
                    encoder.resetPos();
                    enc_pos_temp = 0;
                }
            }
            worker++;
            stepperX.reset();
            encoder.resetPos();
        }

        break;
    case 5: // 5 - печать (работа от энкодера);
        stepperX.setTarget(encoder.getPos() / STEP_DIVIDER);
        break;
    case 6: // 6 - конец печати;
        /* code */
        break;
    case 7: // 7 - Выдвижение каретки;
        /* code */
        break;
    case 8: // от кнопки
        // worker = 0;
        digitalWrite(PIN_PAPER_DAT, !PAPER_DAT_TRUE);
        break;
    default:

        break;
    }

    ////////////////////////////////////////////////////////////////////// Кнопка влево
    if (btnLeft.isPress())
    {
        worker = 8;
        stepperX.setSpeed(-MOTOR_L_BUTTON_SPEED);
    }
    if (btnLeft.isRelease())
    {
        if (!btnLeft.isHolded())
            stepperX.brake();
    }
    if (btnLeft.isHold())
    {
        worker = 8;
        stepperX.setSpeed(-MOTOR_L_AUTO_SPEED);
    }

    /////////////////////////////////////////////////////////////////// Кнопка вправо
    if (btnRight.isPress())
    {
        worker = 8;
        stepperX.setSpeed(MOTOR_R_BUTTON_SPEED);
    }
    if (btnRight.isRelease())
    {
        if (!btnRight.isHolded())
            stepperX.brake();
    }
    if (btnRight.isHold())
    {
        worker = 8;
        stepperX.setSpeed(MOTOR_R_AUTO_SPEED);
    }

    ///////////////////////////////////////////////////////////////// Кнопка вверх
    if (btnUp.isPress())
    {
        // worker = 8;
        stepperY.setSpeed(MOTOR_U_BUTTON_SPEED);
    }
    if (btnUp.isRelease())
    {
        // if (!btnUp.isHolded())
        stepperY.brake();
    }
    // if (btnUp.isHolded())
    // {
    //     worker = 8;
    //     stepperY.setSpeed(MOTOR_U_AUTO_SPEED);
    // }

    //////////////////////////////////////////////////////////////////////////// Кнопка вниз
    if (btnDown.isPress())
    {
        // worker = 8;
        stepperY.setSpeed(-MOTOR_D_BUTTON_SPEED);
        // stepperX.dir = -1;
    }
    if (btnDown.isRelease())
    {
        // if (!btnDown.isHolded())
        stepperY.brake();
    }
    // if (btnDown.isHold())
    // {
    //     worker = 8;
    //     stepperY.setSpeed(-MOTOR_D_AUTO_SPEED);
    // }

    //////////////////////////////////////////////////////////////////////////////Кнопка старт
    if (btnStart.isPress())
    {
        worker = 1;  // Движение домой с лазером
        worker1 = 0; // Сброс первого рабочего
    }
// DDD(laser.state());
// delay(200);
#if DEBUG
    if (millis() > deb_timer)
    {
        deb_timer = millis() + 2000;
        DD("Y Motor Status:");
        DDD(stepperY.getStatus());
        DD("Y Motor Dir:");
        DDD(stepperY.dir);
        DDD();
        DD("X Motor Status:");
        DDD(stepperY.getStatus());
        DD("X Motor Dir:");
        DDD(stepperY.dir);
        DDD();
        DD("WORKER:");
        DDD(worker);
        DD("WORKER1:");
        DDD(worker1);
        // DD("DButton:");
        // DDD(btnDown.state());
        DDD();
    }
#endif

    // Концевики
    if (endRight.state())
    {
        if (stepperX.dir > 0 && stepperX.getStatus())
        {
            stepperX.brake();
            stepperX.reset();
            stepperY.setSpeed(MOTOR_U_BUTTON_SPEED);
            encoder.resetPos();
            switch (worker)
            {
            case 5:
                paper_dat = 0;
                digitalWrite(PIN_PAPER_DAT, !PAPER_DAT_TRUE);
                break;
            case 8:
                // worker = 0;
                break;

            default:
                worker = 0;
                break;
            }
        }
    }

    if (endLeft.state())
    {
        if (stepperX.dir < 0 && stepperX.getStatus())
        {
            stepperX.brake();
            stepperX.reset();
            encoder.resetPos();
            switch (worker)
            {
            case 1:
                switch (worker1)
                {
                case 0:
                    worker1++;
                    break;

                default:
                    break;
                }
                break;
            case 5:
                paper_dat = 0;
                digitalWrite(PIN_PAPER_DAT, !PAPER_DAT_TRUE);
                break;
            case 8:
                // worker = 0;
                break;

            default:
                // worker = 0;
                break;
            }
        }
    }

    if (endUp.state())
    {
        if (stepperY.dir > 0 && stepperY.getStatus())
        {
            stepperY.brake();
            stepperY.reset();
            encoder.resetPos();
            switch (worker)
            {
            case 8:
                // worker = 0;
                break;

            default:
                // worker = 0;
                break;
            }
        }
    }

    if (endDown.state())
    {
        if (stepperY.dir < 0 && stepperY.getStatus())
        {
            stepperY.brake();
            stepperY.reset();
            encoder.resetPos();
            switch (worker)
            {
            case 8:
                // worker = 0;
                break;

            default:
                // worker = 0;
                break;
            }
        }
    }
    stepperY.tick();

    stepperX.tick();
}