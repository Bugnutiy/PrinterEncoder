#pragma once
#include <Arduino.h>
#ifndef PIN_ENC_A
#define PIN_ENC_A 2 // вход А
#endif
#ifndef PIN_ENC_B
#define PIN_ENC_B 3 // вход Б
#endif

#ifndef PINMODE
#define PINMODE INPUT_PULLUP // Какой у нас пинмод
#endif

#ifndef ENC_ALG
#define ENC_ALG 1 // Какой у нас алгоритм (1 - ?)
#endif
#if ENC_ALG == 1
#ifndef ENC_TYPE
#define ENC_TYPE 1
#endif
#endif

#ifndef ENC_READ
#define ENC_READ(PIN) ((PIND >> (PIN)) & 0x01)
#endif
#if ENC_READ == 1
#undef ENC_READ
#define ENC_READ(PIN) ((PIND & (1 << PIN)) >> PIN)
#endif

void encInterruptHandler(); // Внешний обработчик прерываний класса

class Encoder
{
private:
#if ENC_ALG == 1 // Переменные для первого алгоритма
    volatile boolean state0, lastState, turnFlag;
#endif

public:
    volatile int32_t enc_pos = 0;
    Encoder();
    ~Encoder();
    void encInterrupt();
};

Encoder *encoder_ptr;
void encInterruptHandler()
{
    encoder_ptr->encInterrupt();
}

Encoder::Encoder()
{
    pinMode(PIN_ENC_A, PINMODE);
    pinMode(PIN_ENC_B, PINMODE);
    encoder_ptr = this;
#if ENC_ALG == 1 // Прерывание для первого алгоритма
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encInterruptHandler, CHANGE);
#endif
}

Encoder::~Encoder()
{
}

inline void Encoder::encInterrupt()
{
#if ENC_ALG == 1 // Функция прерывания для первого алгоритма
    state0 = ENC_READ(PIN_ENC_A);
    if (state0 != lastState)
    {
#if (ENC_TYPE == 1)
        turnFlag = !turnFlag;
        if (turnFlag)
            enc_pos += (ENC_READ(PIN_ENC_B) != lastState) ? -1 : 1;
#else
        enc_pos += (ENC_READ(PIN_ENC_B) != lastState) ? -1 : 1;
#endif
        lastState = state0;
    }
#endif
}
