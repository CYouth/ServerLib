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
         void handleEvent();//�����ص��¼�����������_callBack�ĳ�Ա����
         void setRevents(int revent);
         int getEvents();
		 int getSockfd();
		 int getIndex();
		 void setIndex(int index);
         void enableReading();//����������¼�
		 void enableWriting();//�������д�¼�
		 void disableWriting();//�������д�¼�
     private:
         void update();//�ڲ��ӿڣ�����ע��״̬
         EventLoop* _eventLoop;//�õ�EventLoop���õ�Epoll
         int _sockfd;//�����socket fd
         int _events;//��ע���¼�
         int _revents;//���ص��¼�
		 int _index;//-1��ʾ֮ǰû��ע���epoll��1��ʾע���
         IChannelCallBack* _callBack;//�ص����������������������fd���¼�
 };

#endif