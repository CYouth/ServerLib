#ifndef ITcpServerCallBack_H
#define ITcpServerCallBack_H

//½Ó¿ÚÀà
class ITcpServerCallBack
{
public:
	void virtual newConnection(int socketfd) = 0;
	void virtual deleteConnection(int socketfd) = 0;
};

#endif