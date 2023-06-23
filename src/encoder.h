#pragma once
#include <Arduino.h>

#ifndef PIN_ENC_A
#define PIN_ENC_A 2 // вход А
#endif
#ifndef PIN_ENC_B
#define PIN_ENC_B 3 // вход Б
#endif

#ifndef ENC_READ
#define ENC_READ(PIN) (PIND & (1 << PIN))
#endif
class Encoder
{
private:
    /* data */
public:
    int32_t enc_pos = 0;
    Encoder();
    ~Encoder();
    static void encInterrupt(void);
};

Encoder::Encoder()
{
}

Encoder::~Encoder()
{
}

void Encoder::encInterrupt()
{
    bool A = ENC_READ(PIN_ENC_A);
    bool B = ENC_READ(PIN_ENC_B);
}
