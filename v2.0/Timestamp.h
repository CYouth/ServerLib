#ifndef TIMERSTAMP_H
#define TIMERSTAMP_H

#include<string>
using namespace std;

//ʱ���������ʱ����ֵ����׼��gettimeofday��������since epoch��΢�뼶��
class Timestamp
{
public:
	Timestamp(double microSeconds);
	~Timestamp();
	int64_t microSecondsSinceEpoch();//getter
	string toString() const;

	static Timestamp now();//����Timerstamp����
	static Timestamp nowAfter(double seconds);//����Timerstamp����
	static double nowMicroSeconds();//��ȡ��ǰʱ��
	static const int kMicroSecondsPerSecond = 1000 * 1000;//1�����xx΢��
private:
	int64_t _microSecondsSinceEpoch;//ʱ�����¼��ʱ�䣬since epoch����ȷ��΢�룬��uint64_t�洢��8���ֽ�
};

//���ز���������Ϊ���漰������
bool operator<(Timestamp l, Timestamp r) {
	return l.microSecondsSinceEpoch() < r.microSecondsSinceEpoch();
}

#endif