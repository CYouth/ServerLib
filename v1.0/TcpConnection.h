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
	void virtual OnIn();//重写，处理连接建立之后的读写
	~TcpConnection();
	void setTcpServer(ITcpServerCallBack* tcpserver);

private:
	EventLoop* _eventLoop;
	int _sockfd;//client fd
	Channel* _channel;//管理_sockfd的channel
	ITcpServerCallBack* _tcpserver;//到时connection要由tcpserver来销毁
};


#endif