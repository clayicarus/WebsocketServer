#include "EchoServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
using namespace muduo::net;

int main() {
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress addr(6011);
    EchoServer server(&loop, addr);
    server.start();
    loop.loop();

    return 0;
}
