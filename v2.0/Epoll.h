#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <vector>

#include"Channel.h"
#include "Define.h"

using namespace std;

class Channel;

class Epoll
{
public:
	Epoll();
	int getFd();
	void update(Channel* channel);//添加或者更新
	void remove(Channel* channel);//删除
	void poll(vector<Channel*>& list);
private:
	int _epollfd;
	struct epoll_event _revents[MAX_EVENTS];//返回事件数组
};



#endif
