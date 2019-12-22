#include "EventLoop.h"

EventLoop::EventLoop()
{
	_epoll = new Epoll();
}

void EventLoop::loop()
{
	//��ѭ��
	while (1)
	{
		//�洢��Ҫ�����channel
		vector<Channel*> list;
		//ʵ�����õ���Epoll��loop����
		_epoll->poll(list);
		//�������
		for (vector<Channel*>::iterator ite = list.begin(); ite != list.end(); ite++) {
			(*ite)->handleEvent();
		}
	}

}

void EventLoop::update(Channel* channel)
{
	_epoll->update(channel);//��ʵ�õ���Epoll��update
}

void EventLoop::remove(Channel* channel)
{
	_epoll->remove(channel);//��ʵ�õ���Epoll��update
}
