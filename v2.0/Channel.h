#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>
#include <iostream>

#include "IChannelCallBack.h"
#include "EventLoop.h"

class EventLoop;

class Channel
 {
     public:     
		 Channel(EventLoop* eventloop, int sockfd);
         ~Channel();
         void setCallBack(IChannelCallBack* callBack);
         void handleEvent();//处理返回的事件，都是利用_callBack的成员函数
         void setRevents(int revent);
         int getEvents();
		 int getSockfd();
		 int getIndex();
		 void setIndex(int index);
         void enableReading();//允许监听读事件
		 void enableWriting();//允许监听写事件
		 void disableWriting();//允许监听写事件
     private:
         void update();//内部接口，更新注册状态
         EventLoop* _eventLoop;//拿到EventLoop就拿到Epoll
         int _sockfd;//管理的socket fd
         int _events;//关注的事件
         int _revents;//返回的事件
		 int _index;//-1表示之前没有注册过epoll，1表示注册过
         IChannelCallBack* _callBack;//回调函数，用这个函数来处理fd的事件
 };

#endif