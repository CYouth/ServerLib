#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>
#include <sys/eventfd.h>

#include"Epoll.h"
#include"IRun.h"
#include"TimerQueue.h"

using namespace std;

class Channel;
class Epoll;

//封装IRun接口和参数
//方便供queueLoop使用
class Runner
{
public:
	Runner(IRun* irun, void* param)
		:_pRun(irun)
		, _param(param) {};
	void doRun()
	{
		_pRun->run(_param);
	}
private:
	IRun* _pRun;
	void* _param;
};

class EventLoop:public IChannelCallBack
{
public:
	EventLoop();
	void loop();//对外接口
	void update(Channel* channel);
	void remove(Channel* channel);
	void virtual handleRead();
	void virtual handleWrite();
	void queueLoop(IRun* run, void* param);//将异步任务放入队列
	//三个创建定时事件函数，返回timer的id标识
	int runAt(IRun* pRun, void* param);
	int runAfter(double delay, IRun* pRun, void* param);
	int runEvery(double interval, IRun* pRun, void* param);
	//根据timer的id标识删除事件
	void cancelTimer(int timerfd);
private:
	Epoll* _epoll;
	int _eventfd;
	Channel* _channel;//管理_eventfd的channel
	vector<Runner> _pendingFunctors;//异步队列
	TimerQueue* _timerQueue;//定时事件的管理类

	int createEventfd();
	void wakeup();//往eventfd写信息，唤醒epoll执行异步任务
	void doPendingFunctors();//执行异步任务队列
};



#endif
