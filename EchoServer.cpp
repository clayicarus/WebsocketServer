//
// Created by clay on 3/23/23.
//

#include "EchoServer.h"
#include "muduo/base/Logging.h"
#include "base/HttpParser.h"
#include "base/StringExtra.h"

using namespace muduo;
using namespace muduo::net;

void EchoServer::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort()
             << "->" << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
}

EchoServer::EchoServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr)
    : server_(loop, listenAddr, "EchoServer")
{
    server_.setMessageCallback([](const auto &conn, auto *buf, auto time){
        EchoServer::onMessage(conn, buf, time);
    });
    server_.setConnectionCallback([](const auto &conn){
        EchoServer::onConnection(conn);
    });
}

void EchoServer::onMessage(const TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time)
{
    auto msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
             << "data received at " << time.toString();

    conn->send(msg);
}

