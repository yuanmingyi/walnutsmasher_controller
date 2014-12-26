#include "Arduino.h"
#include "pin.h"

/** Static members and methods **/

Pin** Pin::pins = NULL;
unsigned long Pin::TimeOut = 30000;

void Pin::Setup()
{
    if (Pin::pins != NULL)
    {
        return;
    }

    Pin::pins = new Pin*[PIN_MAX + 1];

    Pin::pins[PIN_A0] = new Pin(PIN_A0, 0, INPUT);
    Pin::pins[PIN_A1] = new Pin(PIN_A1, 1, INPUT);
    Pin::pins[PIN_A2] = new Pin(PIN_A2, 2, INPUT);
    Pin::pins[PIN_A3] = new Pin(PIN_A3, 3, INPUT);
    Pin::pins[PIN_A4] = new Pin(PIN_A4, 4, INPUT);
    Pin::pins[PIN_A5] = new Pin(PIN_A5, 5, INPUT);
    Pin::pins[PIN_D0] = new Pin(PIN_D0, 0, INPUT);          // Rx
    Pin::pins[PIN_D1] = new Pin(PIN_D1, 1, OUTPUT);         // Tx
    Pin::pins[PIN_D2] = new Pin(PIN_D2, 2, INPUT);
    Pin::pins[PIN_D3] = new Pin(PIN_D3, 3, OUTPUT);         // PWM
    Pin::pins[PIN_D4] = new Pin(PIN_D4, 4, INPUT);
    Pin::pins[PIN_D5] = new Pin(PIN_D5, 5, OUTPUT);         // PWM
    Pin::pins[PIN_D6] = new Pin(PIN_D6, 6, OUTPUT);         // PWM
    Pin::pins[PIN_D7] = new Pin(PIN_D7, 7, INPUT);
    Pin::pins[PIN_D8] = new Pin(PIN_D8, 8, INPUT);
    Pin::pins[PIN_D9] = new Pin(PIN_D9, 9, OUTPUT);         // PWM
    Pin::pins[PIN_D10] = new Pin(PIN_D10, 10, OUTPUT);      // PWM
    Pin::pins[PIN_D11] = new Pin(PIN_D11, 11, OUTPUT);      // PWM
    Pin::pins[PIN_D12] = new Pin(PIN_D12, 12, OUTPUT);
    Pin::pins[PIN_D13] = new Pin(PIN_D13, 13, OUTPUT);      // Led

    for (int i = PIN_D3; i <= PIN_D13; i++)
    {
        Pin* pin = Pin::pins[i];
        pinMode(pin->Number(), pin->Mode());
    }
}

Pin* Pin::GetPin(int pinId)
{
    if (!Pin::CheckId(pinId))
    {
        return NULL;
    }

    return Pin::pins[pinId];
}

int Pin::CheckId(int pinId)
{
    return pinId >= PIN_MIN && pinId <= PIN_MAX;
}

/** Constructors and instance methods **/

Pin::Pin(int id, int number, int mode)
: _id(id), _number(number), _mode(mode)
{
}

int Pin::Number()
{
    return this->_number;
}

int Pin::Id()
{
    return this->_id;
}

int Pin::Mode()
{
    return this->_mode;
}

int Pin::IsAnalog()
{
    return this->_id <= PIN_A5;
}

int Pin::IsPwm()
{
    return this->_id == PIN_D3 || this->_id == PIN_D5 || this->_id == PIN_D6 || this->_id == PIN_D9 || this->_id == PIN_D10 || this->_id == PIN_D11;
}

int Pin::IsDigital()
{
    return this->_id >= PIN_D0;
}

void Pin::Write(int value)
{
    if (this->IsAnalog())
    {
        return;
    }

    if (value >= 255)
    {
        // digital output HIGH
        digitalWrite(this->_number, HIGH);
    }
    else if (value <= 0)
    {
        // digital output low
        digitalWrite(this->_number, LOW);
    }
    else if (this->IsPwm())
    {
        // pwm output
        analogWrite(this->_number, value);
    }
}

int Pin::Read()
{
    if (this->IsAnalog())
    {
        // analog pin
        return analogRead(this->_number);
    }
    else
    {
        // digital pin
        return digitalRead(this->_number);
    }
}

int Pin::PulseWidth(int value)
{
    if (this->IsDigital())
    {
        return pulseIn(this->_number, value, Pin::TimeOut);
    }

    return 0;
}