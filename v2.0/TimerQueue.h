#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <sys/timerfd.h>
#include<set>

#include"Channel.h"
#include"Timer.h"

using namespace std;

class TimerQueue:public IChannelCallBack
{
public:
	TimerQueue(EventLoop* eventLoop);
	~TimerQueue();
	void virtual handleRead();
	void virtual handleWrite();//无逻辑
	//对外的两个接口，addTimer承担runat，runafter，runevery的处理逻辑，这三个函数添加时间事件就行
	//cancelTimer根据timer的id取消时间事件
	int addTimer(IRun* pRun, void* param, Timestamp when, double interval);//添加时间事件，利用AddTimerWrapper异步执行
	void cancelTimer(int timerId);//删除时间事件，利用CancelTimerWrapper异步执行
	int doAddTimer(void* timer);//真正的执行逻辑
	void doCancelTimer(void* timer);//真正的执行逻辑
	
private:
	int _timerfd;//timerfd管理所有定时事件
	Channel* _channel;//管理
	EventLoop* _eventLoop;
	typedef pair<Timestamp, Timer*> Entry;//Timestamp下对应的Timer
	typedef set<Entry> TimerList;//时间事件列表
	TimerList _timerList;
	AddTimerWrapper* _addTimerWrapper;
	CancelTimerWrapper* _cancelTimerWrapper;

	int createTimerfd();//管理timer文件对象
	bool insert(Timer* timer);//这一步可能要更新timerfd的时间
	void resetTimerfd(int timerfd, Timestamp timestamp);//更新timerfd的时间
	struct timespec howMuchTimeFromNow(Timestamp timestamp);//timestamp转化为timespec
	vector<Entry> getExpired(Timestamp now);//相对于当前时间过期的事件
	void reset(vector<Entry> expired, Timestamp now);//过期事件里面可能有循环事件，要将这些事件重新插入列表中
	
};


#endif
