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

#define MAX_READ_SIZE 100 //每次读的时候最大长度

using namespace std;

class TcpConnection :public IChannelCallBack,public IRun
{
public:
	TcpConnection(EventLoop* eventLoop, int sockfd);
	~TcpConnection();
	void virtual handleRead();//重写
	void virtual handleWrite();//重写
	void virtual run();//重写
	void setTcpServer(ITcpServerCallBack* tcpserver);
	void setUser(IMuduoUser* user);
	int getFd();
	string& readFrom();//返回输入缓冲区
	void send(string& msg);//将msg写到socket中
	int getFd();
	void connectionEstablish();//建立连接之后
	
private:
	string _inputBuffer;
	string _outputBuffer;
	EventLoop* _eventLoop;
	int _sockfd;//client fd
	Channel* _channel;//管理_sockfd的channel
	ITcpServerCallBack* _tcpserver;//到时connection要由tcpserver来销毁
	IMuduoUser* _user;
};


#endif