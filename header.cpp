#ifdef WIN32

#include "header.h"
#include <ctime>

extern void pinMode(uint8_t, uint8_t)
{
	// not implemented
}

extern void digitalWrite(uint8_t, uint8_t)
{
	// not implemented
}

int digitalRead(uint8_t)
{
	// not implemented
	return 0;
}

int analogRead(uint8_t)
{
	// not implemented
	return 0;
}

void analogReference(uint8_t mode)
{
	// not implemented
}

void analogWrite(uint8_t, int)
{
	// not implemented
}

unsigned long millis(void)
{
	return (unsigned long)std::round(std::clock_t() * 1000.0 / CLOCKS_PER_SEC);
}

unsigned long micros(void)
{
	return (unsigned long)std::round(std::clock_t() * 1e6 / CLOCKS_PER_SEC);
}

void delay(unsigned long)
{
	// not implemented
}

void delayMicroseconds(unsigned int us)
{
	// not implemented
}

uint16_t makeWord(uint16_t w)
{
	return w;
}

uint16_t makeWord(byte h, byte l)
{
	return (h << 8) & l;
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
	// not implemented
	return 0;
}

#endif

size_t Stream::available()
{

	return 0;
}

Stream::operator boolean()
{
	return true;
}

void Stream::initialize()
{
	if (this->hSerial == NULL)
	{
		this->hSerial = CreateFileW(this->portSpec, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (this->hSerial == INVALID_HANDLE_VALUE)
		{
			std::cerr << "Cannot open serial port " << this->portSpec << std::endl;
			this->hSerial = NULL;
			return;
		}
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(this->hSerial, &dcbSerialParams))
	{
		std::cerr << "Cannot get serial port state" << std::endl;
		return;
	}

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(this->hSerial, &dcbSerialParams))
	{
		std::cerr << "Cannot set serial port state" << std::endl;
		return;
	}

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(this->hSerial, &timeouts))
	{
		std::cerr << "Cannot set serial port timouts" << std::endl;
		return;
	}
}

void Stream::Close()
{
	if (this->hSerial != NULL)
	{
		CloseHandle(this->hSerial);
		this->hSerial = NULL;
	}
}

int Stream::read()
{
	unsigned char b;
	DWORD byteCount = 0;
	if (!ReadFile(this->hSerial, &b, 1, &byteCount, NULL))
	{
		return -1;
	}

	return (int)b;
}

size_t Stream::readBytes(char buff[], size_t length)
{
	DWORD byteCount = 0;
	ReadFile(this->hSerial, buff, length, &byteCount, NULL);
	return byteCount;
}
