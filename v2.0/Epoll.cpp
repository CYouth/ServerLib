#include "Epoll.h"

const int kNew = -1;
const int kAdded = 1;

Epoll::Epoll()
{
	_epollfd = epoll_create(1); //����epollfd
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
	//�¼�
	struct epoll_event ev;
	ev.data.ptr = channel;//����ָ��
	ev.events = events;
	if (channel->getIndex == kNew) {
		epoll_ctl(_epollfd, EPOLL_CTL_ADD, channel->getSockfd(), &ev);//���
		channel->setIndex(kAdded);
	}
	else
	{
		epoll_ctl(_epollfd, EPOLL_CTL_MOD, channel->getSockfd(), &ev);//����
	}
	

	
}

void Epoll::remove(Channel * channel)
{
	int fd = channel->getSockfd();
	epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);//��epoll��ɾ��
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
		ptr->setRevents(_revents[i].events);//��¼���ص��¼�
		list.push_back(ptr);
	}

}
