#include "EchoServer.h"

int main(int args, char** argv)
{
	EventLoop loop;
	EchoServer server(&loop);
	server.start();//¼àÌýfdµÄ×¢²á
	loop.loop();
    return 0;
}
