#ifndef IMUDUOUSER_H
#define IMUDUOUSER_H

//�û�Ӧ�ü̳е��࣬��ע�������¼�
class IMuduoUser
{
public:
	//���ӽ���ʱ
	void virtual onConnection(TcpConnection*) = 0;
	//��Ϣ����ʱ
	void virtual onMessage(TcpConnection*) = 0;
	//��Ϣд���ʱ
	void virtual onWriteComplate(TcpConnection*) = 0;
private:

};

#endif
