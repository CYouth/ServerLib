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
		_eventLoop->remove(_channel);//��epoll��ɾ��
		_tcpserver->deleteConnection(_sockfd);//��tcpserverɾ��������

	}
	else  //readlength>0
	{
		_inputBuffer.append(line, readlength);//append�����뻺����
		//��Ϣ���֪ͨ�û�
		_user->onMessage(this);

	}
}

void TcpConnection::handleWrite() {
	//��ִ�е����������˵������ע��EPOLLOUT�ģ��������ǲ�Ϊ�յ�
	int len = write(_sockfd, _outputBuffer.c_str(), _outputBuffer.size());
	if (len < 0) {
		perror("write error");
		exit(1);
	}
	if (len == _outputBuffer.size()) {
		_outputBuffer.clear();//���
		_channel->disableWriting();//��ʱû������Ҫд�ˣ�ȡ��д�¼�����ֹbusy loop
		//��Ϣд���֪ͨ���첽
		_eventLoop->queueLoop(this);
	}
	else//len<_outputBuffer.size()
	{
		_outputBuffer = _outputBuffer.substr(len);
	}
}

void TcpConnection::run()
{
	//�첽�ص�
	_user->onWriteComplate(this);
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
	//���ӽ���֪ͨ�û�
	_user->onConnection(this);
}

string& TcpConnection::readFrom()
{
	return _inputBuffer;
}

void TcpConnection::send(string& msg)
{
	//Ҫ֪����������������ǿ�ʱ����û��ע��д�¼��ģ�����������Ϊ��ʱ����ע��д�¼���
	//���ȿ���������Ƿ�Ϊ��
	if (_outputBuffer.empty()) {
		//ֱ��write
		int n = write(_sockfd, msg.c_str(), msg.size());//todo��size+1��
		if (n < msg.size()) {//ûд��
			string s = msg.substr(n);//ʣ��ûд��
			_outputBuffer.append(s);
			//enable write
			_channel->enableWriting();
		}
		else//д����
		{
			//��Ϣд���֪ͨ���첽
			_eventLoop->queueLoop(this);
		}
	}
	else
	{
		_outputBuffer.append(msg);
	}
	
}
