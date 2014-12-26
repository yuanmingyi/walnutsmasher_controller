#include "App.h"
#include "hc-sr04.h"

void HcSr04::Distance(void *obj)
{
    HcSr04* pointer = reinterpret_cast<HcSr04*>(obj);
    pointer->_dist = pointer->Detect();
    if (pointer->_monitorCallback)
    {
        pointer->_monitorCallback(pointer);
    }
}

HcSr04::HcSr04(int pinTrig, int pinEcho)
    : _pinTrig(pinTrig)
    , _pinEcho(pinEcho)
    , _timeOut(30000)
    , _interval(100)
    , _taskId(-1)
    , _dist(0)
    , _monitorCallback(NULL)
{
}

unsigned long HcSr04::Detect() const
{
    digitalWrite(this->_pinTrig, LOW);
    delayMicroseconds(3);
    digitalWrite(this->_pinTrig, HIGH);
    delayMicroseconds(12);
    digitalWrite(this->_pinTrig, LOW);

    return pulseIn(this->_pinEcho, HIGH, this->_timeOut);
}

void HcSr04::Setup() const
{
    pinMode(this->_pinTrig, OUTPUT);
    pinMode(this->_pinEcho, INPUT);
}

double HcSr04::Distance(int metric) const
{
    unsigned long dist = this->_dist;
    if (this->_taskId == -1)
    {
        dist = this->Detect();
    }

    if (metric == HcSr04::Centimeter)
    {
        return dist / 58.0;
    }

    if (metric == HcSr04::Inch)
    {
        return dist / 148.0;
    }

	if (metric == HcSr04::Meter)
	{
		return dist * 0.00017;
	}

    return dist;
}

void HcSr04::SetMonitorCallback(void(*monitorCallback)(HcSr04 *))
{
	this->_monitorCallback = monitorCallback;
}

void HcSr04::StartMonitor()
{    
    if (this->_taskId == -1)
    {
        this->_taskId = App.RegisterTask(HcSr04::Distance, this, this->_interval);
    }
}

void HcSr04::EndMonitor()
{
    if (this->_taskId != -1)
    {
        App.RemoveRegisteredTask(this->_taskId);
        this->_taskId = -1;
    }
}

unsigned int HcSr04::Interval() const
{
    return this->_interval;
}

void HcSr04::Interval(unsigned int interval)
{
    if (interval >= 60)
    {
        this->_interval = interval;
    }
}

DataPack HcSr04::Action(unsigned int cmdCode, unsigned int cmdReserved, unsigned int param1, unsigned int param2)
{
	double dist = 0.0;
	unsigned int interval = 0;
	const char* successMessage = 0;
	unsigned int errorCode = RET_SUCCESS;

	switch (cmdCode)
	{
	case CMD_HCSR04:
		successMessage = "HcSr04";
		break;

	case CMD_HCSR04_DIST:
		if (param1 == 1)
		{
			dist = this->Distance(HcSr04::Centimeter);
		}
		else if (param1 == 2)
		{
			dist = this->Distance(HcSr04::Inch);
		}
		else if (param1 == 3)
		{
			dist = this->Distance(HcSr04::Meter);
		}
		else
		{
			dist = this->Distance(HcSr04::Microsecond);
		}

		break;

	case CMD_HCSR04_START:
		this->StartMonitor();
		successMessage = "Start mon";
		break;

	case CMD_HCSR04_END:
		this->EndMonitor();
		successMessage = "End mon";
		break;

	case CMD_HCSR04_INTERVAL:
		interval = makeWord(param1, param2);
		if (interval == 0)
		{
			// interval has to be greater than 0
			interval = this->Interval();
			return DataPack(&interval, sizeof(interval));
		}
		else
		{
			this->Interval(interval);
			successMessage = "Set int";
		}

		break;

	default:
		errorCode = RET_INVALIDPARAM;
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

	if (interval != 0)
	{
		return Command::Success(TYPE_DOUBLE, &interval);
	}

	return Command::Success(TYPE_UINT, &dist);
}