#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include<iostream>
#include <unistd.h>
#include <string>

#include"Channel.h"
#include "Define.h"
#include "ITcpServerCallBack.h"
#include"IMuduoUser.h"
#include"IRun.h"

#define MAX_READ_SIZE 100 //ÿ�ζ���ʱ����󳤶�

using namespace std;

class TcpConnection :public IChannelCallBack,public IRun
{
public:
	TcpConnection(EventLoop* eventLoop, int sockfd);
	~TcpConnection();
	void virtual handleRead();//��д
	void virtual handleWrite();//��д
	void virtual run();//��д
	void setTcpServer(ITcpServerCallBack* tcpserver);
	void setUser(IMuduoUser* user);
	int getFd();
	string& readFrom();//�������뻺����
	void send(string& msg);//��msgд��socket��
	int getFd();
	void connectionEstablish();//��������֮��
	
private:
	string _inputBuffer;
	string _outputBuffer;
	EventLoop* _eventLoop;
	int _sockfd;//client fd
	Channel* _channel;//����_sockfd��channel
	ITcpServerCallBack* _tcpserver;//��ʱconnectionҪ��tcpserver������
	IMuduoUser* _user;
};


#endif