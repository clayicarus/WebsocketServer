//
// Created by clay on 3/24/23.
//

#include "WebsocketServer.h"
#include "base/HttpParser.h"
#include "base/sha1.h"
#include "base/base64.h"

void WebsocketServer::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if(conn->connected()) {
        LOG_INFO << conn->name() << " - " << conn->peerAddress().toIpPort() << " is UP";
        conn->setContext(boost::any_cast<ConnectionStatus>(ConnectionStatus::WAIT_HANDSHAKE));
    } else {
        LOG_INFO << conn->name() << " - " << conn->peerAddress().toIpPort() << " is DOWN";
    }
}

void WebsocketServer::onData(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time)
{
    auto status = boost::any_cast<ConnectionStatus>(conn->getContext());
    switch (status) {
        case ConnectionStatus::WAIT_HANDSHAKE:
            handleHandshake(conn, buf);
            if(buf->readableBytes() == 0)
                break;
        case ConnectionStatus::ESTABLISHED:
            LOG_INFO << conn->name() << " " << conn->peerAddress().toIpPort() << " - "
                << buf->readableBytes() << " bytes received";
            codec_.decodeDataFrame(conn, buf, time);
            break;
        default:
            LOG_FATAL << conn->name() << " " << conn->peerAddress().toIpPort() << " - "
                      << "invalid connection status";
            break;
    }
}

void WebsocketServer::handleHandshake(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf)
{
    LOG_INFO << conn->name() << " " << conn->peerAddress().toIpPort() << " - websocket handshake received";

    static constexpr auto WS_MAX_HANDSHAKE_FRAME_SIZE = 1024 * 1000;
    if(buf->readableBytes() > WS_MAX_HANDSHAKE_FRAME_SIZE) {
        LOG_WARN << conn->name() << " " << conn->peerAddress().toIpPort() << " - receive WS_MAX_HANDSHAKE_FRAME_SIZE";
        conn->shutdown();
        return;
    }

    std::string handshake(buf->peek(), buf->peek() + buf->readableBytes());
    HttpParser parser;
    auto frame_size = parser.parseOne(handshake);
    if(frame_size == 0) { return; }
    buf->retrieve(frame_size);

    if(parser.getMethod() != "GET"
       || parser.getParam("Connection") != "Upgrade"
       || parser.getParam("Upgrade") != "websocket"
       || parser.getParam("Sec-WebSocket-Version") != "13"
       || parser.getParam("Sec-WebSocket-Key").empty()) {
        LOG_WARN << conn->name() << " " << conn->peerAddress().toIpPort() << " - invalid WebSocket request";
        conn->shutdown();
        return;
    }

    static constexpr char MAGIC_KEY[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    auto raw_key = std::string(parser.getParam("Sec-WebSocket-Key")) + MAGIC_KEY;
    std::string sha1_key = SHA1::SHA1HashString(raw_key);
    char accept_key[128] = {0};
    Base64encode(accept_key, sha1_key.c_str(), sha1_key.length());

    std::string rep;
    rep += "HTTP/1.1 101 Switching Protocols"; rep += HttpParser::EOL;
    rep += "Connection: Upgrade"; rep += HttpParser::EOL;
    rep += "Upgrade: websocket"; rep += HttpParser::EOL;
    rep += "Sec-WebSocket-Accept: "; rep += accept_key; rep += HttpParser::EOL;
    auto p = parser.getParam("Sec-WebSocket-Protocol");
    if(!p.empty()) {
        rep += "Sec-WebSocket-Protocol: " + std::string(p) + HttpParser::EOL;
    }
    rep.append(HttpParser::EOL);

    conn->send(rep);
    conn->setContext(boost::any(ConnectionStatus::ESTABLISHED));

    LOG_INFO << conn->name() << " " << conn->peerAddress().toIpPort() << " - websocket connection established";
}

void
WebsocketServer::onMessage(const muduo::net::TcpConnectionPtr &conn, const std::string &msg, muduo::Timestamp recvTime)
{
    LOG_INFO << conn->name() << " " << conn->peerAddress().toIpPort() << " - "
             << "msg received at " << recvTime.toFormattedString() << " decoded";
    messageCallback_(conn, msg, muduo::Timestamp::now());
}
