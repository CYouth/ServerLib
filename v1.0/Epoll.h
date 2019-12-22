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
	void update(Channel* channel);
	void remove(Channel* channel);
	void poll(vector<Channel*>& list);
private:
	int _epollfd;
	struct epoll_event _revents[MAX_EVENTS];//返回事件数组
};



#endif
