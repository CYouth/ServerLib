#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>
#include <sys/eventfd.h>

#include"Epoll.h"
#include"IRun.h"
#include"TimerQueue.h"

using namespace std;

class Channel;
class Epoll;

//��װIRun�ӿںͲ���
//���㹩queueLoopʹ��
class Runner
{
public:
	Runner(IRun* irun, void* param)
		:_pRun(irun)
		, _param(param) {};
	void doRun()
	{
		_pRun->run(_param);
	}
private:
	IRun* _pRun;
	void* _param;
};

class EventLoop:public IChannelCallBack
{
public:
	EventLoop();
	void loop();//����ӿ�
	void update(Channel* channel);
	void remove(Channel* channel);
	void virtual handleRead();
	void virtual handleWrite();
	void queueLoop(IRun* run, void* param);//���첽����������
	//����������ʱ�¼�����������timer��id��ʶ
	int runAt(IRun* pRun, void* param);
	int runAfter(double delay, IRun* pRun, void* param);
	int runEvery(double interval, IRun* pRun, void* param);
	//����timer��id��ʶɾ���¼�
	void cancelTimer(int timerfd);
private:
	Epoll* _epoll;
	int _eventfd;
	Channel* _channel;//����_eventfd��channel
	vector<Runner> _pendingFunctors;//�첽����
	TimerQueue* _timerQueue;//��ʱ�¼��Ĺ�����

	int createEventfd();
	void wakeup();//��eventfdд��Ϣ������epollִ���첽����
	void doPendingFunctors();//ִ���첽�������
};



#endif
