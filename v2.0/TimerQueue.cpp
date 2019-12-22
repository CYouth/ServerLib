#include "TimerQueue.h"

#define UINTPTR_MAX 0xffffffff	//todo，此时假定系统是32位的

TimerQueue::TimerQueue(EventLoop* eventLoop):
	_eventLoop(eventLoop),
	_addTimerWrapper(new AddTimerWrapper(this)),
	_cancelTimerWrapper(new CancelTimerWrapper(this))
{
	_timerfd = createTimerfd();
	_channel = new Channel(_eventLoop, _timerfd);
	_channel->setCallBack(this);
	_channel->enableReading();//注册读事件到epoll

}

TimerQueue::~TimerQueue()
{
}

//定时事件发生处理
void TimerQueue::handleRead()
{
	Timestamp now(Timestamp::now());

	uint64_t howmany;
	ssize_t n = read(_timerfd, &howmany, sizeof(howmany));//read之后timerfd就没有时间了，后续还有时间事件执行记得update
	if (n != sizeof(howmany))
	{
		perror("readTimerfd error");
		exit(1);
	}

	vector<Entry> expired = getExpired(now);//过期事件，这时这些事件已经被删除了
	vector<Entry>::iterator ite;
	for (ite = expired.begin(); ite != expired.end(); ite++)
	{
		ite->second->run();
	}
	reset(expired, now);//过期事件执行完要判断是不是循环事件，后面再update timerfd的时间
}

void TimerQueue::handleWrite()
{
}

int TimerQueue::addTimer(IRun * pRun, void * param, Timestamp when, double interval)
{
	Timer* timer = new Timer(when, pRun, param, interval);
	_eventLoop->queueLoop(_addTimerWrapper, timer);
	return (int)timer;//返回指针
}

void TimerQueue::cancelTimer(int timerId)
{
	_eventLoop->queueLoop(_cancelTimerWrapper, (void*)timerId);
}

int TimerQueue::doAddTimer(void* param)
{
	Timer* timer = static_cast<Timer*>(param);
	bool earliestChanged = insert(timer);//是否需要更新timerfd的时间
	if (earliestChanged)
	{
		resetTimerfd(_timerfd, timer->getStamp());
	}
}

void TimerQueue::doCancelTimer(void* timer)
{
	Timer* pTimer = static_cast<Timer*>(timer);//pTimer是唯一的，找到相同的删除就行
	TimerList::iterator ite;
	for (ite = _timerList.begin(); ite != _timerList.end(); ite++)//遍历列表
	{
		if (ite->second == pTimer)//指针相同
		{
			_timerList.erase(ite);
			delete(pTimer);//记得释放内存
			break;
		}
	}
}

int TimerQueue::createTimerfd()
{
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);//创建timerfd
	if (timerfd < 0)
	{
		perror("failed in timerfd_create");
		exit(1);
	}
	return timerfd;
}

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;//是否需要更新timerfd的时间
	Timestamp timerstamp = timer->getStamp();
	TimerList::iterator it = _timerList.begin();
	if (it == _timerList.end() || timerstamp < it->first)
	{
		earliestChanged = true;
	}
	_timerList.insert(Entry(timerstamp, timer));

	return earliestChanged;
}

void TimerQueue::resetTimerfd(int timerfd, Timestamp timestamp)
{
	struct itimerspec newValue;
	struct itimerspec oldValue;
	bzero(&newValue, sizeof(newValue));
	bzero(&oldValue, sizeof(oldValue));

	newValue.it_value = howMuchTimeFromNow(timestamp);
	int ret = timerfd_settime(timerfd, 0, &newValue, &oldValue);
	if (ret<0)
	{
		perror("timerfd_settime error");
		exit(1);
	}
}

struct timespec TimerQueue::howMuchTimeFromNow(Timestamp timestamp)
{
	//timestamp精确到微秒，timespec精确到纳秒
	int64_t microseconds = timestamp.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();//since epoch要改成since now
	
	//微妙转化为timespec
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);//秒
	ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);//纳秒
	return ts;
}

//获取过期事件，只有过期时间比now大才算不过期
vector<Entry> TimerQueue::getExpired(Timestamp now)
{
	vector<Entry> expired;
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator end = _timerList.lower_bound(sentry);//lower_bound返回第一个大于等于sentry的元素
	copy(_timerList.begin(), end, back_inserter(expired));
	_timerList.erase(_timerList.begin(), end);//删除
	return expired;
}

void TimerQueue::reset(vector<Entry> expired, Timestamp now) {
	vector<Entry>::iterator ite;
	for (ite = expired.begin(); ite != expired.end(); ++ite)//遍历过期事件
	{
		if (ite->second->isRepeat())//是否是循环事件
		{
			ite->second->moveToNext();//更新时间戳
			insert(ite->second);//插入列表
		}
	}

	if (!_timerList.empty())//当列表不为空时，一定要更新timerfd的time
	{
		Timestamp nextTime = _timerList.begin()->second->getStamp();
		resetTimerfd(_timerfd, nextTime);
	}
}