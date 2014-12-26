#ifndef _HEADER_H
#define _HEADER_H

#ifdef WIN32

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <Windows.h>

using namespace std;

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define true 0x1
#define false 0x0

#define PI 3.1415926535897932384626433832795

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);
void analogReference(uint8_t mode);
void analogWrite(uint8_t, int);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

class Stream
{
private:
	const wchar_t * portSpec = L"COM1";
	HANDLE hSerial = NULL;

public:
	size_t available();
	operator boolean();

	template <class T> void print(T v) { std::cout << v; }
	template <class T> void println(T v) { std::cout << v << std::endl; }
	
	void initialize();

	void Close();

	// read a byte
	int read();

	// read a 
	size_t readBytes(char[], size_t length);
};

Stream Serial;

#else

#include "Arduino.h"

#endif	// WIN32

#endif	// _HEADER_H