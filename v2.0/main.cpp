#include "EchoServer.h"

int main(int args, char** argv)
{
	EventLoop loop;
	EchoServer server(&loop);
	server.start();//����fd��ע��
	loop.loop();
    return 0;
}
