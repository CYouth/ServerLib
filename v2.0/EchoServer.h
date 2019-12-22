#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "TcpServer.h"
#include"IMuduoUser.h"

#define MAX_SIZE 4 //用户每次处理的数据长度

//EchoServer应该用户来写
class EchoServer:public IMuduoUser
{
public:
	EchoServer(EventLoop* eventLoop);
	void start();
	void virtual onConnection(TcpConnection* connection);
	void virtual onMessage(TcpConnection* connection);
	void virtual onWriteComplate(TcpConnection* connection);
	
private:
	TcpServer _tcpServer;
	EventLoop* _eventLoop;

};

#endif
