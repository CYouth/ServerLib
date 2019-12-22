#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* eventLoop, int sockfd):
	_eventLoop(eventLoop),
	_sockfd(sockfd)
{
	//用channel来管理fd
	_channel = new Channel(_eventLoop, _sockfd);
	_channel->enableReading();//注册read到epoll中
	_channel->setCallBack(this);
}

void TcpConnection::OnIn()
{
	char line[MAX_LINE];
	int readlength = read(_sockfd, line, MAX_LINE);
	if (readlength < 0)
	{
		perror("read error");
		exit(1);
	}
	else if (readlength == 0)//close
	{
		cout << "read 0 from socket fd:" << _sockfd << endl;
		close(_sockfd);
		_eventLoop->remove(_channel);//在epoll中删除
		_tcpserver->deleteConnection(_sockfd);//由tcpserver删除掉连接

	}
	else  //readlength>0
	{
		write(_sockfd, line, readlength);//echo

	}
}

//析构函数
TcpConnection::~TcpConnection()
{
	delete(_channel);
}

void TcpConnection::setTcpServer(ITcpServerCallBack * tcpserver)
{
	_tcpserver = tcpserver;
}
