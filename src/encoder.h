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
#define ENC_ALG 3 // Какой у нас алгоритм (1 - 3)
#endif
#if ENC_ALG == 1
#ifndef ENC_TYPE
#define ENC_TYPE 2
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
protected:
#if ENC_ALG == 1 // Переменные для первого алгоритма
    volatile boolean _state0, _lastState, _turnFlag;
#endif
#if ENC_ALG == 2 // Переменные для второго алгоритма
    volatile boolean _flag, _resetFlag;
    volatile byte _curState, _prevState;
#endif
#if ENC_ALG == 3               // Переменные для 3 алгоритма
    volatile bool _encFlag = 0; // флаг поворота
    volatile byte _reset = 0, _last = 0;
#endif
    volatile int32_t _enc_pos = 0;

public:
    Encoder();
    ~Encoder();
    void encInterrupt();
    void resetEnc();
    void resetPos();
    int32_t getEncPos();
};

///////////////////////////////////
Encoder *encoder_ptr;
void encInterruptHandler()
{
    encoder_ptr->encInterrupt();
}
///////////////////////////////////

Encoder::Encoder()
{
    pinMode(PIN_ENC_A, PINMODE);
    pinMode(PIN_ENC_B, PINMODE);
    encoder_ptr = this;
#if ENC_ALG == 1 // Прерывание для первого алгоритма
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encInterruptHandler, CHANGE);
#endif
#if ENC_ALG == 2 // прерывания для второго алгоритма
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encInterruptHandler, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encInterruptHandler, CHANGE);
#endif
#if ENC_ALG == 3 // прерывания для 3 алгоритма
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encInterruptHandler, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encInterruptHandler, CHANGE);
#endif
}

Encoder::~Encoder()
{
}

inline void Encoder::encInterrupt()
{
#if ENC_ALG == 1 // Функция прерывания для первого алгоритма
    _state0 = ENC_READ(PIN_ENC_A);
    if (_state0 != _lastState)
    {
#if (ENC_TYPE == 1)
        _turnFlag = !_turnFlag;
        if (_turnFlag)
            _enc_pos += (ENC_READ(PIN_ENC_B) != _lastState) ? -1 : 1;
#else
        _enc_pos += (ENC_READ(PIN_ENC_B) != _lastState) ? -1 : 1;
#endif
        _lastState = _state0;
    }
#endif
#if ENC_ALG == 2 // Функция прерывания для второго алгоритма
    _curState = (ENC_READ(PIN_ENC_A)) | ((ENC_READ(PIN_ENC_B)) << 1);
    if (_resetFlag && _curState == 0b11)
    {
        if (_prevState == 0b10)
            _enc_pos++;
        if (_prevState == 0b01)
            _enc_pos--;
        _resetFlag = 0;
        _flag = true; // Можно юзать как индикатор срабатывания
    }
    if (_curState == 0b00)
        _resetFlag = 1;
    _prevState = _curState;
#endif
#if ENC_ALG == 3                           // Функция прерывания для 3 алгоритма
    byte state = (PIND & 0b00001100) >> 2; // D2 + D3
    if (_reset && state == 0b11)
    {
        int prevCount = _enc_pos;
        if (_last == 0b10)
            _enc_pos++;
        else if (_last == 0b01)
            _enc_pos--;
        if (prevCount != _enc_pos)
            _encFlag = 1; // Индикатор
        _reset = 0;
    }
    if (!state)
        _reset = 1;
    _last = state;
#endif
}

inline void Encoder::resetEnc()
{
#if ENC_ALG == 1 // Переменные для первого алгоритма
    _state0 = 0;
    _lastState = 0;
    _turnFlag = 0;
    _enc_pos = 0;
#endif
#if ENC_ALG == 2 // Переменные для второго алгоритма
    _flag = 0;
    _resetFlag = 0;
    _curState = 0;
    _prevState = 0;
    _enc_pos = 0;
#endif
#if ENC_ALG == 3 // Переменные для 3 алгоритма
    _encFlag = 0; // флаг поворота
    _reset = 0;
    _last = 0;
    _enc_pos = 0;
#endif
}

inline void Encoder::resetPos()
{
    _enc_pos = 0;
}

inline int32_t Encoder::getEncPos()
{
    return _enc_pos;
}
