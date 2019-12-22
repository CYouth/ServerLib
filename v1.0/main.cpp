#include "TcpServer.h"

int main(int args, char** argv)
{
	EventLoop loop;
	TcpServer server(&loop);
	server.start();//¼àÌýfdµÄ×¢²á
	loop.loop();
    return 0;
}
