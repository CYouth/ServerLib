#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>

#include"Epoll.h"

using namespace std;

class Channel;
class Epoll;

class EventLoop
{
public:
	EventLoop();
	void loop();
	void update(Channel* channel);
	void remove(Channel* channel);
private:
	Epoll* _epoll;
	
};



#endif
