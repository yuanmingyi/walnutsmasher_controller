#include "L298.h"

// test if the velocity is valid,
// return -1 if velocity is larger than 0 and smaller than MIN_VELOCITY;
// return 0 if velocity is 0 or between MIN_VELOCITY and MAX_VELOCITY
// return 1 if velocity is larger than MAX_VELOCITY
int TestVelocity(int velocity)
{
	int v = abs(velocity);
	return (v > 0 && v < L298::MIN_VELOCITY) ? -1 : (v > L298::MAX_VELOCITY ? 1 : 0);
}

void SetMotor(int velocity, int pin1, int pin2, int pinEna)
{
	int s1 = velocity > 0 ? HIGH : LOW;
	int s2 = velocity >= 0 ? LOW : HIGH;
	digitalWrite(pin1, s1);
	digitalWrite(pin2, s2);
	analogWrite(pinEna, abs(velocity));
}

void Turn(int *velocity, int step, int pin1, int pin2, int pinEna)
{
	*velocity -= step;
	int d = TestVelocity(*velocity);
	if (d < 0)
	{
		*velocity = -L298::MIN_VELOCITY;
	}
	else if (d > 0)
	{
		*velocity = -L298::MAX_VELOCITY;
	}

	SetMotor(*velocity, pin1, pin2, pinEna);
}

void AddStep(int *vmin, int *vmax, int step)
{
	int v1 = *vmin, v2 = *vmax, v11, v22 = v2 + step;

	// assume 0 <= v1 <= v2, step != 0
	if (v22 > L298::MAX_VELOCITY)
	{
		v22 = L298::MAX_VELOCITY;
	}
	else if (v22 < L298::MIN_VELOCITY)
	{
		v22 = step < 0 ? 0 : L298::MIN_VELOCITY;
	}

	v11 = (v1 == v2) ? v22 : (v1 * v22 / v2);
	if (v11 < L298::MIN_VELOCITY)
	{
		v11 = 0;
	}

	*vmin = v11;
	*vmax = v22;
}

L298::L298(int pinIn1, int pinIn2, int pinIn3, int pinIn4, int pinEna1, int pinEna2)
	: _pinIn1(pinIn1)
	, _pinIn2(pinIn2)
	, _pinIn3(pinIn3)
	, _pinIn4(pinIn4)
	, _pinEna1(pinEna1)
	, _pinEna2(pinEna2)
	, _velocity1(0)
	, _velocity2(0)
	, _step(10)
{
}

void L298::Setup() const
{
	pinMode(this->_pinIn1, OUTPUT);
	pinMode(this->_pinIn2, OUTPUT);
	pinMode(this->_pinIn3, OUTPUT);
	pinMode(this->_pinIn4, OUTPUT);
	pinMode(this->_pinEna1, OUTPUT);
	pinMode(this->_pinEna2, OUTPUT);
}

void L298::Step(int step)
{
	if (step > 0)
	{
		this->_step = step;
	}
}

void L298::SetMotor1(int velocity)
{
	this->_velocity1 = velocity;
	SetMotor(velocity, this->_pinIn1, this->_pinIn2, this->_pinEna1);
}

void L298::SetMotor2(int velocity)
{
	this->_velocity2 = velocity;
	SetMotor(velocity, this->_pinIn3, this->_pinIn4, this->_pinEna2);
}

void L298::MoveStraight(int velocity)
{
	this->SetMotor1(velocity);
	this->SetMotor2(velocity);
}

void L298::Turn1()
{
	Turn(&this->_velocity1, this->_step, this->_pinIn1, this->_pinIn2, this->_pinEna1);
}

void L298::Turn2()
{
	Turn(&this->_velocity2, this->_step, this->_pinIn3, this->_pinIn4, this->_pinEna2);
}

void L298::SpeedUp()
{
	this->AddStep(this->_step);
}

void L298::SlowDown()
{
	this->AddStep(-this->_step);
}

void L298::Swing1(int velocity)
{
	SetMotor1(-velocity);
	SetMotor2(velocity);
}

void L298::Swing2(int velocity)
{
	SetMotor1(velocity);
	SetMotor2(-velocity);
}

void L298::Stop1()
{
	digitalWrite(this->_pinEna1, 0);
	this->_velocity1 = 0;
}

void L298::Stop2()
{
	digitalWrite(this->_pinEna2, 0);
	this->_velocity2 = 0;
}

void L298::Stop()
{
	this->Stop1();
	this->Stop2();
}

void L298::AddStep(int step)
{
	int v1 = abs(this->_velocity1);
	int v2 = abs(this->_velocity2);
	if (v1 < v2)
	{
		::AddStep(&v1, &v2, step);
	}
	else
	{
		::AddStep(&v2, &v1, step);
	}

	this->SetMotor1(this->_velocity1 >= 0 ? v1 : -v1);
	this->SetMotor2(this->_velocity2 >= 0 ? v2 : -v2);
}

DataPack L298::Action(unsigned int cmdCode, unsigned int cmdReserved, unsigned int param1, unsigned int param2)
{
	const char *successMessage = 0;
	unsigned int errorCode = RET_SUCCESS;
	int step = 0;

	switch (cmdCode)
	{
	case CMD_CAR:
		successMessage = "L298";
		break;

	case CMD_CAR_MOVE:
		if (param1 == 0)
		{
			this->MoveStraight(param2);
			successMessage = "Move fw";
		}
		else
		{
			this->MoveStraight(-param2);
			successMessage = "Move bw";
		}

		break;

	case CMD_CAR_STOP:
		if (param1 == 1)
		{
			this->Stop1();
			successMessage = "Stop1";
		}
		else if (param1 == 2)
		{
			this->Stop2();
			successMessage = "Stop2";
		}
		else
		{
			this->Stop();
			successMessage = "Stop both";
		}

		break;

	case CMD_MOTOR1_SET:
		if (param1 == 0)
		{
			this->SetMotor1(param2);
			successMessage = "Set1 pos";
		}
		else
		{
			this->SetMotor1(-param2);
			successMessage = "Set1 neg";
		}

		break;

	case CMD_MOTOR2_SET:
		if (param1 == 0)
		{
			this->SetMotor2(param2);
			successMessage = "Set2 pos";
		}
		else
		{
			this->SetMotor2(-param2);
			successMessage = "Set2 neg";
		}

		break;

	case CMD_CAR_TURNDIR:
		if (param1 == 1)
		{
			this->Turn1();
			successMessage = "Turn1";
		}
		else if (param1 == 2)
		{
			this->Turn2();
			successMessage = "Turn2";
		}
		else
		{
			errorCode = RET_INVALIDPARAM;
		}

		break;

	case CMD_CAR_STEP:
		if (param1 == 0)
		{
			step = this->Step();
		}
		else
		{
			this->Step(param2);
			successMessage = "Set step";
		}

		break;

	case CMD_CAR_SPEEDUP:
		this->SpeedUp();
		successMessage = "SpeedUp";
		break;

	case CMD_CAR_SLOWDOWN:
		this->SlowDown();
		successMessage = "SlowDown";
		break;

	case CMD_CAR_SWING:
		if (param1 == 1)
		{
			this->Swing1(param2);
			successMessage = "Swing1";
		}
		else if (param1 == 2)
		{
			this->Swing2(param2);
			successMessage = "Swing2";
		}
		else
		{
			errorCode = RET_INVALIDPARAM;
		}

		break;

	default:
		errorCode = RET_INVALIDCODE;
		break;
	}

	if (errorCode != RET_SUCCESS)
	{
		return Command::Fail(errorCode);
	}

	if (successMessage != 0)
	{
		return Command::Success(successMessage);
	}

	return Command::Success(TYPE_INT, &step);
}