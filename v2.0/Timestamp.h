#ifndef TIMERSTAMP_H
#define TIMERSTAMP_H

#include<string>
using namespace std;

//时间戳，管理时间数值，基准是gettimeofday，精度是since epoch的微秒级别
class Timestamp
{
public:
	Timestamp(double microSeconds);
	~Timestamp();
	int64_t microSecondsSinceEpoch();//getter
	string toString() const;

	static Timestamp now();//创建Timerstamp对象
	static Timestamp nowAfter(double seconds);//创建Timerstamp对象
	static double nowMicroSeconds();//获取当前时间
	static const int kMicroSecondsPerSecond = 1000 * 1000;//1秒等于xx微秒
private:
	int64_t _microSecondsSinceEpoch;//时间戳记录的时间，since epoch，精确到微秒，用uint64_t存储，8个字节
};

//重载操作符，因为有涉及到排序
bool operator<(Timestamp l, Timestamp r) {
	return l.microSecondsSinceEpoch() < r.microSecondsSinceEpoch();
}

#endif