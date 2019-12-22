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

void TcpConnection::handleRead()
{
	char line[MAX_READ_SIZE];
	int readlength = read(_sockfd, line, MAX_READ_SIZE);
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
		_inputBuffer.append(line, readlength);//append到输入缓冲区
		//消息到达，通知用户
		_user->onMessage(this);

	}
}

void TcpConnection::handleWrite() {
	//能执行到这个函数，说明是有注册EPOLLOUT的，缓冲区是不为空的
	int len = write(_sockfd, _outputBuffer.c_str(), _outputBuffer.size());
	if (len < 0) {
		perror("write error");
		exit(1);
	}
	if (len == _outputBuffer.size()) {
		_outputBuffer.clear();//清空
		_channel->disableWriting();//此时没有数据要写了，取消写事件，防止busy loop
		//消息写完毕通知，异步
		_eventLoop->queueLoop(this);
	}
	else//len<_outputBuffer.size()
	{
		_outputBuffer = _outputBuffer.substr(len);
	}
}

void TcpConnection::run()
{
	//异步回调
	_user->onWriteComplate(this);
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

void TcpConnection::setUser(IMuduoUser * user)
{
	_user = user;
}

int TcpConnection::getFd()
{
	return _sockfd;
}

void TcpConnection::connectionEstablish()
{
	//连接建立通知用户
	_user->onConnection(this);
}

string& TcpConnection::readFrom()
{
	return _inputBuffer;
}

void TcpConnection::send(string& msg)
{
	//要知道，当输出缓冲区是空时，是没有注册写事件的，当缓冲区不为空时，是注册写事件的
	//首先看输出缓冲是否为空
	if (_outputBuffer.empty()) {
		//直接write
		int n = write(_sockfd, msg.c_str(), msg.size());//todo，size+1？
		if (n < msg.size()) {//没写完
			string s = msg.substr(n);//剩下没写的
			_outputBuffer.append(s);
			//enable write
			_channel->enableWriting();
		}
		else//写完了
		{
			//消息写完毕通知，异步
			_eventLoop->queueLoop(this);
		}
	}
	else
	{
		_outputBuffer.append(msg);
	}
	
}
