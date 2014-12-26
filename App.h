#ifndef _APP_H
#define _APP_H

#include "header.h"

#ifndef NULL
#define NULL 0
#endif

class _App
{
    static _App _instance;
    static const int _maxTaskCount = 16;

    int _currentTaskCount;
    boolean _overflow;
    unsigned long _ticks;

    struct TaskType
    {
        void (*taskFunc)(void*);
        void *param;
        unsigned long interval;
        unsigned long nextLaunchTime;
        boolean overflow;

        TaskType();
    };

    TaskType _taskList[_maxTaskCount];

    _App();

public:
    /// <Summary>
    /// get the unique instance of this class
    /// </Summary>
    static _App & GetInstance();

    /// <summary>
    /// register a task into the task list
    /// </summary>
    /// <param name = "task">delegate to execute the task</Param>
    /// <param name = "param">object passed to the task delegate</param>
    /// <param name = "interval">interval (milliseconds) between two adjacent execution. 0 interval means one-shot task</param>
    /// <return type = "int">the registed task id if succeeded, or -1 if failed</return>
    int RegisterTask(void (*task)(void*), void *param, unsigned long interval);

    /// <summary>
    /// remove the registered task given by task id from the task list
    /// </summary>
    void RemoveRegisteredTask(int taskId);

    /// <summary>
    /// Main loop, run the tasklist and handle serial input. It should be exclusively invoked in loop() body.
    /// </summary>
    void Run();

    void (*SerialEvent)(int availableCount);
};

extern _App App;

#endif
