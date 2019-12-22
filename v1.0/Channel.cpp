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
	if (_revents & EPOLLIN) {//������¼�
		_callBack->OnIn();//ͨ��_callBack�ĳ�Ա�����Ĵ���
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
	_events|= EPOLLIN;//Ĭ����ˮƽģʽ
	update();
}

void Channel::update()
{
	_eventLoop->update(this);
}
