#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

//抽象类
class IChannelCallBack
{
public:
	//管理的fd有读写两个事件
	void virtual handleRead() = 0;
	void virtual handleWrite() = 0;
};

#endif