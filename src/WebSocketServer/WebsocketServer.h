//
// Created by clay on 3/24/23.
//

#ifndef WEBSOCKET_SERVER_WEBSOCKETSERVER_H
#define WEBSOCKET_SERVER_WEBSOCKETSERVER_H

#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"

class WebsocketServer {
public:
    enum class CONNECTION_STATUS {
        WAIT_HANDSHAKE, ESTABLISHED
    };
    WebsocketServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr)
        : server_(loop, addr, "WebsocketServer")
    {
        server_.setConnectionCallback([this](const auto &conn){
            WebsocketServer::onConnect(conn);
        });
        server_.setMessageCallback([this](const auto &conn, auto *buf, auto time){
            WebsocketServer::onMessage(conn, buf, time);
        });
    }
    void start()
    {
        LOG_INFO << server_.name() << " - start";
        server_.start();
    }
private:
    void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time);
    void onConnect(const muduo::net::TcpConnectionPtr &conn);
    void handleHandshake();
    void handle

    muduo::net::TcpServer server_;
};


#endif //WEBSOCKET_SERVER_WEBSOCKETSERVER_H
