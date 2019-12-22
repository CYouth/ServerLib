#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<map>

#include "Acceptor.h"
#include "TcpConnection.h"
#include "ITcpServerCallBack.h"
#include "EventLoop.h"

using namespace std;

class TcpServer:private ITcpServerCallBack
{
public:
	TcpServer(EventLoop* eventLoop);
	~TcpServer();
	void start();//准备工作，监听fd的注册等

private:
	Acceptor* _acceptor;
	map<int, TcpConnection*> _connections;//所有连接
	EventLoop* _eventLoop;
	void virtual newConnection(int socketfd);
	void virtual deleteConnection(int socketfd);
};

#endif


