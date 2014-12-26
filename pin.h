#ifndef _PIN_H
#define _PIN_H

#ifndef NULL
#define NULL 0
#endif

#include "Arduino.h"

// pin id
#define PIN_A0 0x00
#define PIN_A1 0x01
#define PIN_A2 0x02
#define PIN_A3 0x03
#define PIN_A4 0x04
#define PIN_A5 0x05
#define PIN_D0 0x06
#define PIN_D1 0x07
#define PIN_D2 0x08
#define PIN_D3 0x09
#define PIN_D4 0x0A
#define PIN_D5 0x0B
#define PIN_D6 0x0C
#define PIN_D7 0x0D
#define PIN_D8 0x0E
#define PIN_D9 0x0F
#define PIN_D10 0x10
#define PIN_D11 0x11
#define PIN_D12 0x12
#define PIN_D13 0x13
#define PIN_MIN PIN_A0
#define PIN_MAX PIN_D13

class Pin
{
public:
    static void Setup();
    static Pin* GetPin(int pinId);
    static int CheckId(int pinId);

    static unsigned long TimeOut;

    // get the properties of the pin
    int Number();
    int Id();
    int Mode();

    // test type of the pin
    int IsAnalog();
    int IsPwm();
    int IsDigital();

    // write a signal (byte) to the pin
    void Write(int value);

    // read a signal (int) from the pin
    int Read();

    // read pulse width from the pin
    int PulseWidth(int value);

private:
    int _number;
    int _id;
    int _mode;

    Pin(int id, int number, int mode);

    static Pin **pins;
};

#endif