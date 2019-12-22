#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>

#include "Define.h"
#include "ITcpServerCallBack.h"
#include "Channel.h"

using namespace std;

class Acceptor:public IChannelCallBack
{
public:
	Acceptor(EventLoop*);
	int createAndListen();
	void virtual OnIn();//��д������������
	void setTcpServer(ITcpServerCallBack* tcpserver);
	~Acceptor();

private:
	EventLoop* _eventLoop;
	int _sockfd;//����fd
	Channel* _channel;//����_sockfd��channel
	ITcpServerCallBack* _tcpserver;//��ʱconnectionҪ��tcpserver������
};

#endif
