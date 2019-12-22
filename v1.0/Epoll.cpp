#include "Epoll.h"

Epoll::Epoll()
{
	_epollfd = epoll_create(1); //创建epollfd
	if (_epollfd < 0) {
		perror("epoll_create error");
		exit(1);
	}
}

int Epoll::getFd()
{
	return _epollfd;
}

void Epoll::update(Channel * channel)
{
	int events = channel->getEvents();
	//事件
	struct epoll_event ev;
	ev.data.ptr = channel;//传入指针
	ev.events = events;

	epoll_ctl(_epollfd, EPOLL_CTL_ADD, channel->getSockfd(), &ev);//注册
}

void Epoll::remove(Channel * channel)
{
	int fd = channel->getSockfd();
	epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);//在epoll中删除
}

void Epoll::poll(vector<Channel*>& list)
{
	int fds = epoll_wait(_epollfd, _revents, MAX_EVENTS, -1);
	if (fds == -1)
	{
		perror("epoll_wait error");
		exit(1);
	}

	for (int i = 0; i < fds; i++) {
		Channel* ptr = (Channel*)_revents[i].data.ptr;
		ptr->setRevents(_revents[i].events);//记录返回的事件
		list.push_back(ptr);
	}

}
