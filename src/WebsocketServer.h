//
// Created by clay on 3/24/23.
//

#ifndef WEBSOCKET_SERVER_WEBSOCKETSERVER_H
#define WEBSOCKET_SERVER_WEBSOCKETSERVER_H

#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"

class WebsocketServer {
public:
    WebsocketServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr)
        : server_(loop, addr, "WebsocketServer")
    {
        server_.setConnectionCallback([this](const auto &conn){
            WebsocketServer::onConnect(conn);
        });
    }
    void start()
    {
        LOG_INFO << server_.name() << " - start";
        server_.start();
    }
private:
    void onConnect(const muduo::net::TcpConnectionPtr &conn);

    muduo::net::TcpServer server_;
};


#endif //WEBSOCKET_SERVER_WEBSOCKETSERVER_H
