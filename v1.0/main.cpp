#include "TcpServer.h"

int main(int args, char** argv)
{
	EventLoop loop;
	TcpServer server(&loop);
	server.start();//����fd��ע��
	loop.loop();
    return 0;
}
