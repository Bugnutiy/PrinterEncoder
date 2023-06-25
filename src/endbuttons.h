#pragma once
#include "Arduino.h"
#include "functions.h"

class endBtn
{
private:
    byte _pin;
    bool _state, _truth, _statePrev;
    unsigned long _debounce = 0;
    uint16_t _debounce_end = 20;

public:
    endBtn(byte pin, byte pin_mode, byte truth, uint16_t debounce_end = 20);
    bool state();
    bool statePrev();
    void statePrevSet();
    void statePrevSet(bool);
    // ~endBtn();
};

endBtn::endBtn(byte pin, byte pin_mode, byte truth, uint16_t debounce_end)
{
    pinMode(pin, pin_mode);
    _pin = pin;
    _truth = !truth;
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
        _debounce = millis() + _debounce_end;
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
    _state = (fastRead(_pin) ^ _truth);
    return _state;
}

inline bool endBtn::statePrev()
{
    return _statePrev;
}

inline void endBtn::statePrevSet()
{
    _statePrev = _state;
}
inline void endBtn::statePrevSet(bool st)
{
    _statePrev = st;
}
