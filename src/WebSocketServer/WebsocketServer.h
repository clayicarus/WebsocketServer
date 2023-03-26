//
// Created by clay on 3/24/23.
//

#ifndef WEBSOCKET_SERVER_WEBSOCKETSERVER_H
#define WEBSOCKET_SERVER_WEBSOCKETSERVER_H

#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "DataFrameCodec.h"

class WebsocketServer {
public:
    WebsocketServer(const WebsocketServer &) = delete;
    WebsocketServer &operator=(const WebsocketServer&) = delete;
    typedef std::function<void(const muduo::net::TcpConnectionPtr&, const std::string&, muduo::Timestamp)> MessageCallback;
    enum class ConnectionStatus {
        WAIT_HANDSHAKE, ESTABLISHED
    };
    WebsocketServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &addr)
        : server_(loop, addr, "WebsocketServer"),
          codec_([this](const auto &conn, const auto &msg, auto time)
            { onMessage(conn, msg, time); }),
          messageCallback_([](const auto &, const auto &, auto){})
    {
        server_.setConnectionCallback([this](const auto &conn)
            { onConnection(conn); });
        server_.setMessageCallback([this](const auto &conn, auto *buf, auto time)
            { onData(conn, buf, time); });
    }
    void start()
    {
        LOG_INFO << server_.name() << " - start";
        server_.start();
    }
    void setMessageCallback(MessageCallback callback) { messageCallback_ = std::move(callback); }
private:
    void onData(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time);
    void onConnection(const muduo::net::TcpConnectionPtr &conn);
    void onMessage(const muduo::net::TcpConnectionPtr &conn, const std::string &msg, muduo::Timestamp recvTime);
    void handleHandshake(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf);

    DataFrameCodec codec_;
    muduo::net::TcpServer server_;
    MessageCallback messageCallback_;
};


#endif //WEBSOCKET_SERVER_WEBSOCKETSERVER_H
