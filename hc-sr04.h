#ifndef _HC_SR04_H
#define _HC_SR04_H

#include "header.h"
#include "Command.h"

class HcSr04 : public ICommandExecutor
{
    int _pinTrig;
    int _pinEcho;
    unsigned long _timeOut;
    unsigned long _interval;
    int _taskId;
    unsigned long _dist;
    void (*_monitorCallback)(HcSr04* obj);

    unsigned long Detect() const;

public:
	virtual DataPack Action(unsigned int cmdCode, unsigned int cmdReserved, unsigned int param1, unsigned int param2);

public:
    // distance metrics
    static const int Microsecond = 0;
    static const int Centimeter = 1;
    static const int Inch = 2;
	static const int Meter = 3;

    static void Distance(void *obj);

    HcSr04(int pinTrig, int pinEcho);

    void Setup() const;

    /// <summary>
    /// 1. if the monitor is on, then return the distance of the last detection in given metric
    /// 2. if the monitor is off, then trigger a instant detection and return the distance in given metric
    /// <param name="metric">the metric in which the distance value is</param>
    /// </summary>
    double Distance(int metric) const;

	void SetMonitorCallback(void (*monitorCallback)(HcSr04*));

    /// <summary>
    /// start monitoring the distance, periodly triggering the detection with certain interval (can be set)
    /// </summary>
    void StartMonitor();

    /// <summary>
    /// stop monitoring the distance
    /// </summary>
    void EndMonitor();

    /// <summary>
    /// get current interval for detecting distance
    /// </summary>
    /// <return type="ulong">interval value in millisecond</param>
    unsigned int Interval() const;

    /// <summary>
    /// set interval for detecting distance
    /// </summary>
    /// <param name = "interval">interval value in millisecond, should not be smaller than 60ms</param>
    void Interval(unsigned int interval);
};

#endif
