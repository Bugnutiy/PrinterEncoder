#pragma once
#include "Arduino.h"
#include "functions.h"
#ifndef DEBOUNCE_END
#define DEBOUNCE_END 20
#endif

class endBtn
{
private:
    byte _pin;
    bool _state, _truth;
    unsigned long _debounce = 0;
    uint16_t _debounce_end = 20;

public:
    endBtn(byte pin, byte pin_mode, byte truth, uint16_t debounce_end = 20);
    bool state();
    // ~endBtn();
};

endBtn::endBtn(byte pin, byte pin_mode, byte truth, uint16_t debounce_end)
{
    pinMode(pin, pin_mode);
    _pin = pin;
    _truth = 1 ^ truth;
    _debounce_end = debounce_end;
}
bool endBtn::state()
{
    if (_state)
    {
        _state = (fastRead(_pin) ^ _truth);
        if (_state)
        {
            return _state; // 1
        }
        _debounce = millis() + DEBOUNCE_END;
        return _state; // 0
    }

    if (_debounce)
    {
        if (millis() >= _debounce)
        {
            _debounce = 0;
            _state = (fastRead(_pin) ^ _truth);
        }
        return _state;
    }
    _state = (fastRead(_pin) == _truth);
    return _state;
}
