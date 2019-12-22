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
		_pRun->run(_param);//ִ��ʱ���¼�
	}
	//�Ƿ��������¼�
	bool isRepeat()
	{
		return _interval > 0.0;
	}
	//�����¼�����ʱ���
	void moveToNext()
	{
		_stamp = Timestamp::nowAfter(_interval);
	}
private:
	Timestamp _stamp;//�¼�����ִ�е�ʱ���
	IRun* _pRun;//ʱ���¼�ִ�к���
	void* _param;//��������
	double _interval;//seconds�������¼���ִ�м����������ʱ��Ϊ0
	
};

//�첽���񣬱�����ִ��TimerQueue�ĺ���
class AddTimerWrapper : public IRun
{
public:
	AddTimerWrapper(TimerQueue* pQueue)
		:_pQueue(pQueue) {};
	
	virtual void run(void* param)//param��timer*
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