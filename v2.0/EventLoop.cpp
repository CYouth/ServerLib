#include "EventLoop.h"

EventLoop::EventLoop()
{
	_epoll = new Epoll();
	_eventfd = createEventfd();
	_channel = new Channel(this, _eventfd);
	_channel->setCallBack(this);
	_channel->enableReading();//ע�ᵽepollfd
}

void EventLoop::loop()
{
	//��ѭ��
	while (1)
	{
		//�洢��Ҫ�����channel
		vector<Channel*> list;
		//ʵ�����õ���Epoll��loop����
		_epoll->poll(list);
		//�������
		for (vector<Channel*>::iterator ite = list.begin(); ite != list.end(); ite++) {
			(*ite)->handleEvent();
		}

		doPendingFunctors();
	}

}

void EventLoop::update(Channel* channel)
{
	_epoll->update(channel);//��ʵ�õ���Epoll��update
}

void EventLoop::remove(Channel* channel)
{
	_epoll->remove(channel);//��ʵ�õ���Epoll��update
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	int n = ::read(_eventfd, &one, sizeof(one));
	if (n != sizeof(one))
	{
		perror("read eventfd error");
		exit(1);
	}
}

void EventLoop::handleWrite()
{
}

int EventLoop::createEventfd()
{
	int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);//һ��Ҫ����Ϊ������
	if (eventfd < 0)
	{
		perror("create eventfd error");
		exit(1);
	}
	return eventfd;

}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	int n = write(_eventfd, &one, sizeof(one));
	if (n != sizeof(one))
	{
		perror("write eventfd error");
		exit(1);
	}
}

void EventLoop::doPendingFunctors()
{
	vector<Runner> tmp;
	tmp.swap(_pendingFunctors);//swap�кܴ������
	for (vector<Runner>::iterator ite = tmp.begin(); ite != tmp.end(); ite++) {
		(ite)->doRun();
	}
}

void EventLoop::queueLoop(IRun* run,void* param)
{
	_pendingFunctors.push_back(Runner(run, param));
	wakeup();
}

int EventLoop::runAt(IRun * pRun, void * param)
{
	return _timerQueue->addTimer(pRun, param, Timestamp::now(), 0);
}

int EventLoop::runAfter(double delay, IRun * pRun, void * param)
{
	return _timerQueue->addTimer(pRun, param, Timestamp::nowAfter(delay), 0);
}

int EventLoop::runEvery(double interval, IRun * pRun, void * param)
{
	return _timerQueue->addTimer(pRun, param, Timestamp::nowAfter(interval), interval);
}

void EventLoop::cancelTimer(int timerfd)
{
	_timerQueue->cancelTimer(timerfd);
}

