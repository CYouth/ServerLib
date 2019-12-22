#include "Channel.h"

Channel::Channel(EventLoop* eventloop, int sockfd):
	_eventLoop(eventloop),
	_sockfd(sockfd)
{
	
}

Channel::~Channel()
{
}

void Channel::setCallBack(IChannelCallBack * callBack)
{
	_callBack = callBack;
}

void Channel::handleEvent()
{
	if (_revents & EPOLLIN) {//处理读事件
		_callBack->OnIn();//通过_callBack的成员函数的处理
	}
}

void Channel::setRevents(int revent)
{
	_revents = revent;
}

int Channel::getEvents()
{
	return _events;
}

int Channel::getSockfd()
{
	return _sockfd;
}


void Channel::enableReading()
{
	_events|= EPOLLIN;//默认是水平模式
	update();
}

void Channel::update()
{
	_eventLoop->update(this);
}
