#include "Timestamp.h"

Timestamp::Timestamp(double microSeconds) :
	_microSecondsSinceEpoch(microSeconds)
{
}

Timestamp::~Timestamp()
{
}

int64_t Timestamp::microSecondsSinceEpoch()
{
	return _microSecondsSinceEpoch;
}

string Timestamp::toString() const
{
	/*char buf[32] = {0};
	int64_t seconds = _microSecondsSinceEpoch / kMicroSecondsPerSecond;
	int64_t microseconds = _microSecondsSinceEpoch % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
	return buf;*/
	return "todo";
}

Timestamp Timestamp::now()
{
	return Timestamp(nowMicroSeconds());
}

Timestamp Timestamp::nowAfter(double seconds)
{
	return Timestamp(nowMicroSeconds() + seconds*kMicroSecondsPerSecond);
}

double Timestamp::nowMicroSeconds()
{
	struct timeval tv;//精确到微秒
	gettimeofday(&tv, NULL);//获取当前时间距离epoch
	int64_t seconds = tv.tv_sec;
	return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}
