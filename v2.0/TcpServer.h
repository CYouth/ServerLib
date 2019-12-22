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

class IMuduoUser;

class TcpServer:private ITcpServerCallBack
{
public:
	TcpServer(EventLoop* eventLoop);
	~TcpServer();
	void start();//准备工作，监听fd的注册等
	void setUser(IMuduoUser* user);

private:
	Acceptor* _acceptor;
	map<int, TcpConnection*> _connections;//所有连接
	EventLoop* _eventLoop;
	void virtual newConnection(int socketfd);
	void virtual deleteConnection(int socketfd);
	IMuduoUser* _user;//为了传递给TcpConnection
};

#endif


