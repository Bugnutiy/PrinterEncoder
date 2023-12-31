#pragma once
#include "Arduino.h"
/**
 * @brief Функция быстрого чтения для uno/nano
 *
 * @param pin Номер пина
 * @return состояние пина
 * @return true
 * @return false
 */
bool fastRead(uint8_t pin)
{
    if (pin < 8)
        // return bitRead(PINB, pin);
        return ((PIND >> (pin)) & 0x01);
    else if (pin < 14)
        // return bitRead(PINB, pin - 8);
        return ((PINB >> (pin - 8)) & 0x01);
    else if (pin < 20)
        // return bitRead(PINC, pin - 14);
        return ((PINC >> (pin - 14)) & 0x01);
    return 0;
}


