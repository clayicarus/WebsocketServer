#include "EchoServer.h"
#include "muduo/net/EventLoop.h"
#include "WebSocketServer/WebsocketServer.h"

using namespace muduo::net;

int main()
{
    // Logger::setLogLevel(Logger::DEBUG);
    EventLoop loop;
    EchoServer echoServer(&loop, InetAddress(6011));
    WebsocketServer websocketServer(&loop, InetAddress(9000));
    websocketServer.setMessageCallback([]
        (const auto &conn, const auto &msg, auto time){
            printf("%s", msg.c_str());
        });

    echoServer.start();
    websocketServer.start();
    loop.loop();

    return 0;
}