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
	void start();//׼������������fd��ע���

private:
	Acceptor* _acceptor;
	map<int, TcpConnection*> _connections;//��������
	EventLoop* _eventLoop;
	void virtual newConnection(int socketfd);
	void virtual deleteConnection(int socketfd);
};

#endif


