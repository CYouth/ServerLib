#include "Channel.h"

Channel::Channel(EventLoop* eventloop, int sockfd):
	_eventLoop(eventloop),
	_sockfd(sockfd),
	_index(-1),
	_events(0)
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
	//�ȴ�����¼��ٴ���д�¼�
	if (_revents & EPOLLIN) {//������¼�
		_callBack->handleRead();
	}
	if (_revents & EPOLLOUT) {//����д�¼�
		_callBack->handleWrite();
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

int Channel::getIndex()
{
	return _index;
}

void Channel::setIndex(int index)
{
	_index = index;
}


void Channel::enableReading()
{
	_events|= EPOLLIN;//Ĭ����ˮƽģʽ
	update();
}

void Channel::enableWriting()
{
	_events |= EPOLLOUT;
	update();
}

void Channel::disableWriting()
{
	_events &= ~EPOLLOUT;
	update();
}

void Channel::update()
{
	_eventLoop->update(this);
}
