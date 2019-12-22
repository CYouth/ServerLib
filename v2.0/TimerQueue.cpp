#include "TimerQueue.h"

#define UINTPTR_MAX 0xffffffff	//todo����ʱ�ٶ�ϵͳ��32λ��

TimerQueue::TimerQueue(EventLoop* eventLoop):
	_eventLoop(eventLoop),
	_addTimerWrapper(new AddTimerWrapper(this)),
	_cancelTimerWrapper(new CancelTimerWrapper(this))
{
	_timerfd = createTimerfd();
	_channel = new Channel(_eventLoop, _timerfd);
	_channel->setCallBack(this);
	_channel->enableReading();//ע����¼���epoll

}

TimerQueue::~TimerQueue()
{
}

//��ʱ�¼���������
void TimerQueue::handleRead()
{
	Timestamp now(Timestamp::now());

	uint64_t howmany;
	ssize_t n = read(_timerfd, &howmany, sizeof(howmany));//read֮��timerfd��û��ʱ���ˣ���������ʱ���¼�ִ�мǵ�update
	if (n != sizeof(howmany))
	{
		perror("readTimerfd error");
		exit(1);
	}

	vector<Entry> expired = getExpired(now);//�����¼�����ʱ��Щ�¼��Ѿ���ɾ����
	vector<Entry>::iterator ite;
	for (ite = expired.begin(); ite != expired.end(); ite++)
	{
		ite->second->run();
	}
	reset(expired, now);//�����¼�ִ����Ҫ�ж��ǲ���ѭ���¼���������update timerfd��ʱ��
}

void TimerQueue::handleWrite()
{
}

int TimerQueue::addTimer(IRun * pRun, void * param, Timestamp when, double interval)
{
	Timer* timer = new Timer(when, pRun, param, interval);
	_eventLoop->queueLoop(_addTimerWrapper, timer);
	return (int)timer;//����ָ��
}

void TimerQueue::cancelTimer(int timerId)
{
	_eventLoop->queueLoop(_cancelTimerWrapper, (void*)timerId);
}

int TimerQueue::doAddTimer(void* param)
{
	Timer* timer = static_cast<Timer*>(param);
	bool earliestChanged = insert(timer);//�Ƿ���Ҫ����timerfd��ʱ��
	if (earliestChanged)
	{
		resetTimerfd(_timerfd, timer->getStamp());
	}
}

void TimerQueue::doCancelTimer(void* timer)
{
	Timer* pTimer = static_cast<Timer*>(timer);//pTimer��Ψһ�ģ��ҵ���ͬ��ɾ������
	TimerList::iterator ite;
	for (ite = _timerList.begin(); ite != _timerList.end(); ite++)//�����б�
	{
		if (ite->second == pTimer)//ָ����ͬ
		{
			_timerList.erase(ite);
			delete(pTimer);//�ǵ��ͷ��ڴ�
			break;
		}
	}
}

int TimerQueue::createTimerfd()
{
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);//����timerfd
	if (timerfd < 0)
	{
		perror("failed in timerfd_create");
		exit(1);
	}
	return timerfd;
}

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;//�Ƿ���Ҫ����timerfd��ʱ��
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
	//timestamp��ȷ��΢�룬timespec��ȷ������
	int64_t microseconds = timestamp.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();//since epochҪ�ĳ�since now
	
	//΢��ת��Ϊtimespec
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);//��
	ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);//����
	return ts;
}

//��ȡ�����¼���ֻ�й���ʱ���now����㲻����
vector<Entry> TimerQueue::getExpired(Timestamp now)
{
	vector<Entry> expired;
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator end = _timerList.lower_bound(sentry);//lower_bound���ص�һ�����ڵ���sentry��Ԫ��
	copy(_timerList.begin(), end, back_inserter(expired));
	_timerList.erase(_timerList.begin(), end);//ɾ��
	return expired;
}

void TimerQueue::reset(vector<Entry> expired, Timestamp now) {
	vector<Entry>::iterator ite;
	for (ite = expired.begin(); ite != expired.end(); ++ite)//���������¼�
	{
		if (ite->second->isRepeat())//�Ƿ���ѭ���¼�
		{
			ite->second->moveToNext();//����ʱ���
			insert(ite->second);//�����б�
		}
	}

	if (!_timerList.empty())//���б�Ϊ��ʱ��һ��Ҫ����timerfd��time
	{
		Timestamp nextTime = _timerList.begin()->second->getStamp();
		resetTimerfd(_timerfd, nextTime);
	}
}