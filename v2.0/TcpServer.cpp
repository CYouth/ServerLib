#include "TcpServer.h"

TcpServer::TcpServer(EventLoop* eventLoop):
	_eventLoop(eventLoop)
{

}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
	//创建accpetor
	_acceptor = new Acceptor(_eventLoop);
	_acceptor->setTcpServer(this);
	
}

void TcpServer::setUser(IMuduoUser * user)
{
	_user = user;
}

void TcpServer::newConnection(int socketfd)
{
	fcntl(socketfd, F_SETFL, O_NONBLOCK); //加入epoll之前非阻塞
	TcpConnection* ptr = new TcpConnection(_eventLoop,socketfd);
	ptr->setTcpServer(this);
	ptr->setUser(_user);
	ptr->connectionEstablish();
	_connections[socketfd] = ptr;//添加到map中

}

//删除连接
void TcpServer::deleteConnection(int socketfd)
{
	TcpConnection* ptr = _connections[socketfd];
	delete(ptr);
	//记得在集合中删除
	_connections.erase(socketfd);
}

