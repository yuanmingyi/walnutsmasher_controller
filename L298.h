#ifndef _L298_H
#define _L298_H

#include "header.h"
#include "Command.h"

class L298 : public ICommandExecutor
{
	// in1 - in4 for direction controal. ena1 and ena2 for speed control
	int _pinIn1;
	int _pinIn2;
	int _pinIn3;
	int _pinIn4;
	int _pinEna1;
	int _pinEna2;
	int _velocity1;
	int _velocity2;
	int _step;

public:
	virtual DataPack Action(unsigned int cmdCode, unsigned int cmdReserved, unsigned int param1, unsigned int param2);

public:
	static const int MAX_VELOCITY = 0xFF;
	static const int MIN_VELOCITY = 0x40;

	L298(int pinIn1, int pinIn2, int pinIn3, int pinIn4, int pinEna1, int pinEna2);

	void Setup() const;
	int Step() const { return this->_step; }
	void Step(int step);

	// velocity: -255 ~ 255
	void SetMotor1(int velocity);
	void SetMotor2(int velocity);
	void MoveStraight(int velocity);
	void Turn1();
	void Turn2();
	void SpeedUp();
	void SlowDown();
	void Swing1(int velocity);
	void Swing2(int velocity);
	void Stop1();
	void Stop2();
	void Stop();

private:
	void AddStep(int step);
};

#endif
