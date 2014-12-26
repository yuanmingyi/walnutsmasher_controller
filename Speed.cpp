#include "Speed.h"
#include "header.h"

Speed::Speed(int pinOut1, int pinOut2, double diameter, int stripOnWheel)
	: _pinOut1(pinOut1), _pinOut2(pinOut2), _stripOnWheel(stripOnWheel)
{
	this->_k = diameter * PI / this->_stripOnWheel / 2.0;
}

void Speed::Setup() const
{
	pinMode(this->_pinOut1, INPUT);
	pinMode(this->_pinOut2, INPUT);
}

void Speed::Get(double *l1, double *l2, unsigned long interval) const
{
	int count1 = 0, count2 = 0;
	int pin1 = digitalRead(this->_pinOut1);
	int pin2 = digitalRead(this->_pinOut2);
	unsigned long end = millis() + interval;
	while (millis() < end)
	{
		int v1 = digitalRead(this->_pinOut1);
		int v2 = digitalRead(this->_pinOut2);

		if (v1 != pin1)
		{
			count1++;
			pin1 = v1;
		}

		if (v2 != pin2)
		{
			count2++;
			pin2 = v2;
		}
	}

	*l1 = count1 * this->_k * 1000 / interval;
	*l2 = count2 * this->_k * 1000 / interval;
}

double Speed::Get1() const
{
	double l1, l2;
	this->Get(&l1, &l2);
	return l1;
}

double Speed::Get2() const
{
	double l1, l2;
	this->Get(&l1, &l2);
	return l2;
}

double Speed::GetMean() const
{
	double l1, l2;
	this->Get(&l1, &l2);
	return (l1 + l2) / 2.0;
}

DataPack Speed::Action(unsigned int cmdCode, unsigned int cmdReserved, unsigned int param1, unsigned int param2)
{
	double speed = 0;
	switch (cmdCode)
	{
	case CMD_SPEED:
		return Command::Success("Speed");
		break;

	case CMD_SPEED_GET:
		if (param1 == 1)
		{
			speed = this->Get1();
		}
		else if (param1 == 2)
		{
			speed = this->Get2();
		}
		else
		{
			speed = this->GetMean();
		}

		return Command::Success(TYPE_DOUBLE, &speed);
		break;

	default:
		break;
	}

	return Command::Fail(RET_INVALIDCODE);
}
