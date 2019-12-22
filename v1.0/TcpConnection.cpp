#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* eventLoop, int sockfd):
	_eventLoop(eventLoop),
	_sockfd(sockfd)
{
	//��channel������fd
	_channel = new Channel(_eventLoop, _sockfd);
	_channel->enableReading();//ע��read��epoll��
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
		_eventLoop->remove(_channel);//��epoll��ɾ��
		_tcpserver->deleteConnection(_sockfd);//��tcpserverɾ��������

	}
	else  //readlength>0
	{
		write(_sockfd, line, readlength);//echo

	}
}

//��������
TcpConnection::~TcpConnection()
{
	delete(_channel);
}

void TcpConnection::setTcpServer(ITcpServerCallBack * tcpserver)
{
	_tcpserver = tcpserver;
}
