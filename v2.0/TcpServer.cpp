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
	//����accpetor
	_acceptor = new Acceptor(_eventLoop);
	_acceptor->setTcpServer(this);
	
}

void TcpServer::setUser(IMuduoUser * user)
{
	_user = user;
}

void TcpServer::newConnection(int socketfd)
{
	fcntl(socketfd, F_SETFL, O_NONBLOCK); //����epoll֮ǰ������
	TcpConnection* ptr = new TcpConnection(_eventLoop,socketfd);
	ptr->setTcpServer(this);
	ptr->setUser(_user);
	ptr->connectionEstablish();
	_connections[socketfd] = ptr;//��ӵ�map��

}

//ɾ������
void TcpServer::deleteConnection(int socketfd)
{
	TcpConnection* ptr = _connections[socketfd];
	delete(ptr);
	//�ǵ��ڼ�����ɾ��
	_connections.erase(socketfd);
}

