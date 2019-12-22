#include "EchoServer.h"

EchoServer::EchoServer(EventLoop * eventLoop):
	_eventLoop(eventLoop),
	_tcpServer(_eventLoop)
{
	_tcpServer.setUser(this);
}

void EchoServer::start()
{
	_tcpServer.start();
}

void EchoServer::onConnection(TcpConnection* connection)
{
	cout << "connection build,fd is:" << connection->getFd() << endl;
}

//��Ϣ�����ʱ��Ϣ�Ѿ���Ӧ�ò��input buffer
void EchoServer::onMessage(TcpConnection * connection)
{
	//ÿ�δ���MAX_SIZE
	while (connection->readFrom().size() > MAX_SIZE) {//��>=�ᵼ�»���������ΪMAX_SIZEʱ��ȡ��������Խ��
		//��������
		string s = connection->readFrom().substr(0, MAX_SIZE);
		connection->readFrom() = connection->readFrom().substr(MAX_SIZE);
		//д��ȥ��echo
		connection->send(s);
	}
}

void EchoServer::onWriteComplate(TcpConnection * connection)
{
	cout << "connection data write complete,fd is:" << connection->getFd() << endl;
}


