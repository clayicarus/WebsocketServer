//
// Created by clay on 3/26/23.
//

#include "muduo/net/TcpServer.h"

#ifndef WEBSOCKET_SERVER_DATAFRAMECODEC_H
#define WEBSOCKET_SERVER_DATAFRAMECODEC_H


class DataFrameCodec {
public:
    DataFrameCodec(const DataFrameCodec&) = delete;
    DataFrameCodec &operator=(const DataFrameCodec&) = delete;

    enum class DataFrameInfo {
        MIN_HEADER_LENGTH = 2,

        OPCODE_IS_PART_FRAME = 0x0,
        OPCODE_IS_TEXT_FRAME = 0x1,
        OPCODE_IS_BINARY_FRAME = 0x2,
        OPCODE_DISCONNECT = 0x8,
        OPCODE_IS_PING = 0x9,
        OPCODE_IS_PONG = 0xa,
    };

    typedef std::function<void(const muduo::net::TcpConnectionPtr &, const std::string &, muduo::Timestamp)>
            MessageCallback;
    explicit DataFrameCodec(MessageCallback cb)
        : callback_(std::move(cb))
    {}
    void decodeDataFrame(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time);
private:
    // return parse header length
    int32_t parseHeader(const muduo::net::Buffer *buf);
    // call at payload init, retrieve a whole frame payload to this->payload_, buf must bigger than payload_length
    uint64_t parsePayload(const muduo::net::Buffer *buf);
    MessageCallback callback_;

    uint8_t fin_;
    uint8_t opcode_;
    uint64_t payload_length_;
    uint8_t mask_;
    uint8_t masking_key_[4];

    std::string payload_;
};


#endif //WEBSOCKET_SERVER_DATAFRAMECODEC_H
