#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <sys/timerfd.h>
#include<set>

#include"Channel.h"
#include"Timer.h"

using namespace std;

class TimerQueue:public IChannelCallBack
{
public:
	TimerQueue(EventLoop* eventLoop);
	~TimerQueue();
	void virtual handleRead();
	void virtual handleWrite();//���߼�
	//����������ӿڣ�addTimer�е�runat��runafter��runevery�Ĵ����߼����������������ʱ���¼�����
	//cancelTimer����timer��idȡ��ʱ���¼�
	int addTimer(IRun* pRun, void* param, Timestamp when, double interval);//���ʱ���¼�������AddTimerWrapper�첽ִ��
	void cancelTimer(int timerId);//ɾ��ʱ���¼�������CancelTimerWrapper�첽ִ��
	int doAddTimer(void* timer);//������ִ���߼�
	void doCancelTimer(void* timer);//������ִ���߼�
	
private:
	int _timerfd;//timerfd�������ж�ʱ�¼�
	Channel* _channel;//����
	EventLoop* _eventLoop;
	typedef pair<Timestamp, Timer*> Entry;//Timestamp�¶�Ӧ��Timer
	typedef set<Entry> TimerList;//ʱ���¼��б�
	TimerList _timerList;
	AddTimerWrapper* _addTimerWrapper;
	CancelTimerWrapper* _cancelTimerWrapper;

	int createTimerfd();//����timer�ļ�����
	bool insert(Timer* timer);//��һ������Ҫ����timerfd��ʱ��
	void resetTimerfd(int timerfd, Timestamp timestamp);//����timerfd��ʱ��
	struct timespec howMuchTimeFromNow(Timestamp timestamp);//timestampת��Ϊtimespec
	vector<Entry> getExpired(Timestamp now);//����ڵ�ǰʱ����ڵ��¼�
	void reset(vector<Entry> expired, Timestamp now);//�����¼����������ѭ���¼���Ҫ����Щ�¼����²����б���
	
};


#endif
