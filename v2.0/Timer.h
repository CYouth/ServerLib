#ifndef TIMER_H
#define TIMER_H

#include"Timestamp.h"

class Timer
{
public:
	Timer(Timestamp stamp, IRun* pRun, void* param, double interval)
		:_stamp(stamp)
		, _pRun(pRun)
		,_param(param)
		, _interval(interval)
	{}
	Timestamp getStamp()
	{
		return _stamp;
	}
	void run()
	{
		_pRun->run(_param);//执行时间事件
	}
	//是否是周期事件
	bool isRepeat()
	{
		return _interval > 0.0;
	}
	//周期事件更新时间戳
	void moveToNext()
	{
		_stamp = Timestamp::nowAfter(_interval);
	}
private:
	Timestamp _stamp;//事件即将执行的时间戳
	IRun* _pRun;//时间事件执行函数
	void* _param;//函数参数
	double _interval;//seconds，周期事件的执行间隔，非周期时间为0
	
};

//异步任务，本质是执行TimerQueue的函数
class AddTimerWrapper : public IRun
{
public:
	AddTimerWrapper(TimerQueue* pQueue)
		:_pQueue(pQueue) {};
	
	virtual void run(void* param)//param是timer*
	{
		_pQueue->doAddTimer(param);
	};
private:
	TimerQueue* _pQueue;
};

class CancelTimerWrapper : public IRun
{
public:
	CancelTimerWrapper(TimerQueue* pQueue)
		:_pQueue(pQueue) {};

	virtual void run(void* timer)
	{
		_pQueue->doCancelTimer(timer);
	}
private:
	TimerQueue* _pQueue;
};

#endif