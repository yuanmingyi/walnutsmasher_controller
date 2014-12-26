#include "App.h"

_App _App::_instance;
_App App = _App::GetInstance();

_App::TaskType::TaskType()
	: taskFunc(NULL), param(NULL), interval(0), nextLaunchTime(0), overflow(false)
{
}

_App& _App::GetInstance()
{
	return _App::_instance;
}

_App::_App()
	: _currentTaskCount(0), _overflow(false), _ticks(0), SerialEvent(NULL)
{
}

int _App::RegisterTask(void(*task)(void*), void * param, unsigned long interval)
{
	if (this->_currentTaskCount < _App::_maxTaskCount && task != NULL)
	{
		for (int i = 0; i < _App::_maxTaskCount; i++)
		{
			TaskType *pTask = this->_taskList + i;
			if (pTask->taskFunc == NULL)
			{
				pTask->taskFunc = task;
				pTask->param = param;
				pTask->interval = interval;
				pTask->nextLaunchTime = millis();
				pTask->overflow = this->_overflow;
				this->_currentTaskCount++;
				return i;
			}
		}
	}

	return -1;
}

void _App::RemoveRegisteredTask(int taskId)
{
	if (taskId >= 0 && taskId < _App::_maxTaskCount && this->_taskList[taskId].taskFunc != NULL)
	{
		this->_taskList[taskId].taskFunc = NULL;
		this->_currentTaskCount--;
	}
}

void _App::Run()
{
	for (TaskType *pTask = this->_taskList; pTask - this->_taskList < _App::_maxTaskCount; pTask++)
	{
		if (pTask->taskFunc == NULL)
		{
			continue;
		}

		unsigned long ticks = millis();
		if (ticks < this->_ticks)
		{
			this->_overflow = !this->_overflow;
		}

		this->_ticks = ticks;

		if (this->_overflow == pTask->overflow && ticks >= pTask->nextLaunchTime)
		{
			pTask->taskFunc(pTask->param);
			if (pTask->interval <= 0)
			{
				// one-shot task
				this->RemoveRegisteredTask(pTask - this->_taskList);
			}
			else
			{
				pTask->nextLaunchTime = ticks + pTask->interval;
				if (pTask->nextLaunchTime < pTask->interval)
				{
					pTask->overflow = !pTask->overflow;
				}
			}
		}
	}
}