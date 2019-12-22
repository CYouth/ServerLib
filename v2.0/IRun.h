#ifndef IRUN_H
#define IRUN_H

//用户应该继承的类，关注三个半事件
class IRun
{
public:
	//任务
	void virtual run(void* param) = 0;
private:

};

#endif
