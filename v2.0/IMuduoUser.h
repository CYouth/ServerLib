#ifndef IMUDUOUSER_H
#define IMUDUOUSER_H

//用户应该继承的类，关注三个半事件
class IMuduoUser
{
public:
	//连接建立时
	void virtual onConnection(TcpConnection*) = 0;
	//消息到达时
	void virtual onMessage(TcpConnection*) = 0;
	//消息写完成时
	void virtual onWriteComplate(TcpConnection*) = 0;
private:

};

#endif
