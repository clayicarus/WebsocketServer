//
// Created by clay on 3/23/23.
//

#ifndef WEBSOCKET_SERVER_ECHOSERVER_H
#define WEBSOCKET_SERVER_ECHOSERVER_H

#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"

class EchoServer {
public:
    EchoServer &operator=(const EchoServer &) = delete;
    EchoServer(const EchoServer &) = delete;

    EchoServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr);
    void start()
    {
        LOG_INFO << server_.name() << " - start";
        server_.start();
    }
private:
    static void onConnection(const muduo::net::TcpConnectionPtr &conn);
    static void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time);

    muduo::net::TcpServer server_;
};


#endif //WEBSOCKET_SERVER_ECHOSERVER_H
