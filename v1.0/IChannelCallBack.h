#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

//抽象类
class IChannelCallBack
{
public:
	//必须让子类重写
	void virtual OnIn() = 0;
};

#endif