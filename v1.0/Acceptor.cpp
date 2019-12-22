#include "Acceptor.h"

Acceptor::Acceptor(EventLoop* eventLoop):
	_eventLoop(eventLoop)
{
	_sockfd = createAndListen();

	//用channel来管理fd
	_channel = new Channel(_eventLoop, _sockfd);
	_channel->enableReading();//注册read到epoll中
	_channel->setCallBack(this);
}

//创建监听socket
int Acceptor::createAndListen()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);//监听socket
	fcntl(listenfd, F_SETFL, O_NONBLOCK); // 加入epoll的socket要先设置为非阻塞
	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));//SO_REUSEADDR

	//绑定的地址
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(11111);	//port

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind error");
		exit(1);
	}

	if (-1 == listen(listenfd, MAX_LISTENFD))
	{
		perror("listen error");
		exit(1);
	}
	return listenfd;
}

void Acceptor::OnIn()
{
	struct sockaddr_in cliaddr;//客户端地址
	socklen_t clilen = sizeof(struct sockaddr_in);

	int connfd = accept(_sockfd, (sockaddr*)&cliaddr, (socklen_t*)&clilen);//每次accept一个连接
	if (connfd > 0)
	{
		cout << "new connection from "
			<< "[" << inet_ntoa(cliaddr.sin_addr)
			<< ":" << ntohs(cliaddr.sin_port) << "]"
			<< " accept socket fd:" << connfd
			<< endl;
	}
	else
	{
		perror("accept error");
		exit(1);
	}

	_tcpserver->newConnection(connfd);
}

void Acceptor::setTcpServer(ITcpServerCallBack * tcpserver)
{
	_tcpserver = tcpserver;
}

Acceptor::~Acceptor()
{
	delete(_channel);
}
