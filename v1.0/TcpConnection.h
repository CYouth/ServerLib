#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include<iostream>
#include <unistd.h>

#include"Channel.h"
#include "Define.h"
#include "ITcpServerCallBack.h"

using namespace std;

class TcpConnection :public IChannelCallBack
{
public:
	TcpConnection(EventLoop* eventLoop, int sockfd);
	void virtual OnIn();//��д���������ӽ���֮��Ķ�д
	~TcpConnection();
	void setTcpServer(ITcpServerCallBack* tcpserver);

private:
	EventLoop* _eventLoop;
	int _sockfd;//client fd
	Channel* _channel;//����_sockfd��channel
	ITcpServerCallBack* _tcpserver;//��ʱconnectionҪ��tcpserver������
};


#endif