#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

//������
class IChannelCallBack
{
public:
	//�����fd�ж�д�����¼�
	void virtual handleRead() = 0;
	void virtual handleWrite() = 0;
};

#endif