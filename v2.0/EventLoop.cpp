#include "EventLoop.h"

EventLoop::EventLoop()
{
	_epoll = new Epoll();
	_eventfd = createEventfd();
	_channel = new Channel(this, _eventfd);
	_channel->setCallBack(this);
	_channel->enableReading();//注册到epollfd
}

void EventLoop::loop()
{
	//主循环
	while (1)
	{
		//存储需要处理的channel
		vector<Channel*> list;
		//实际上用的是Epoll的loop函数
		_epoll->poll(list);
		//逐个处理
		for (vector<Channel*>::iterator ite = list.begin(); ite != list.end(); ite++) {
			(*ite)->handleEvent();
		}

		doPendingFunctors();
	}

}

void EventLoop::update(Channel* channel)
{
	_epoll->update(channel);//其实用的是Epoll的update
}

void EventLoop::remove(Channel* channel)
{
	_epoll->remove(channel);//其实用的是Epoll的update
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
	int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);//一定要设置为非阻塞
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
	tmp.swap(_pendingFunctors);//swap有很大的作用
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

