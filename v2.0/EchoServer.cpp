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

//消息到达，此时消息已经在应用层的input buffer
void EchoServer::onMessage(TcpConnection * connection)
{
	//每次处理MAX_SIZE
	while (connection->readFrom().size() > MAX_SIZE) {//用>=会导致缓冲区长度为MAX_SIZE时截取产生数组越界
		//读出数据
		string s = connection->readFrom().substr(0, MAX_SIZE);
		connection->readFrom() = connection->readFrom().substr(MAX_SIZE);
		//写回去，echo
		connection->send(s);
	}
}

void EchoServer::onWriteComplate(TcpConnection * connection)
{
	cout << "connection data write complete,fd is:" << connection->getFd() << endl;
}


