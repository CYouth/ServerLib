#include "EventLoop.h"

EventLoop::EventLoop()
{
	_epoll = new Epoll();
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
