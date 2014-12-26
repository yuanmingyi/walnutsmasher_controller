#ifndef _SPEED_H
#define _SPEED_H

#include "Command.h"

class Speed : public ICommandExecutor
{
	int _pinOut1;
	int _pinOut2;
	int _stripOnWheel;
	double _k;

public:
	virtual DataPack Action(unsigned int cmdCode, unsigned int cmdReserved, unsigned int param1, unsigned int param2);

public:
	Speed(int pinOut1, int pinOut2, double diameter = 6.6, int stripOnWheel = 20);
	void Setup() const;
	void Get(double *l1, double *l2, unsigned long interval = 50) const;
	double Get1() const;
	double Get2() const;
	double GetMean() const;
};

#endif
