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
	void virtual handleRead();//重写，处理新连接
	void virtual handleWrite();//重写
	void setTcpServer(ITcpServerCallBack* tcpserver);

private:
	EventLoop* _eventLoop;
	int _sockfd;//监听fd
	Channel* _channel;//管理_sockfd的channel
	ITcpServerCallBack* _tcpserver;//到时connection要由tcpserver来创建
};

#endif
