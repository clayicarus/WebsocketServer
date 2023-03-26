//
// Created by clay on 3/26/23.
//

#include "DataFrameCodec.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/base/Logging.h"

void DataFrameCodec::decodeDataFrame(const muduo::net::TcpConnectionPtr &conn,
                                     muduo::net::Buffer *buf,
                                     muduo::Timestamp time)
{
    while(buf->readableBytes() > static_cast<int>(DataFrameInfo::MIN_HEADER_LENGTH)) {
        auto header_length = parseHeader(buf);
        if(header_length < 0) {
            LOG_ERROR << conn->name() << " " << conn->peerAddress().toIpPort() << " - "
                      << "extend websocket not support";
            conn->shutdown();
            return;
        } else if(header_length == 0) {
            // not enough length
            return;
        }
        if(buf->readableBytes() < header_length + payload_length_) {
            return;
        }
        buf->retrieve(header_length);
        buf->retrieve(parsePayload(buf));

        if(fin_ == 1) {
            callback_(conn, payload_, time);
            payload_.clear();
        } else {
            
            LOG_DEBUG << "part of dataframe";
        }
    }
}

int32_t DataFrameCodec::parseHeader(const muduo::net::Buffer *buf)
{
    int32_t cur_head_len = 0;
    if(buf->readableBytes() < static_cast<int>(DataFrameInfo::MIN_HEADER_LENGTH)) {
        return 0;
    }
    uint8_t fin_opcode = buf->peek()[0];
    fin_ = fin_opcode >> 7 & 1;
    uint8_t rsv = fin_opcode >> 4 & 0x7;
    if(rsv != 0) {
        // not support
        return -1;
    }
    opcode_ = fin_opcode & 0x0f;
    uint8_t mask_len = buf->peek()[1];
    mask_ = mask_len >> 7 & 1;
    uint8_t length_type = mask_len & 0x7f;
    // fin and opcode
    cur_head_len += static_cast<int>(DataFrameInfo::MIN_HEADER_LENGTH);

    if(length_type == 126) {
        // followed 2 bytes is payload length
        if(buf->readableBytes() < static_cast<int>(DataFrameInfo::MIN_HEADER_LENGTH) + 2) {
            return 0;
        }
        auto temp = *reinterpret_cast<const uint16_t*>(buf->peek() + cur_head_len);
        payload_length_ = muduo::net::sockets::networkToHost16(temp);
        cur_head_len += 2;
    } else if(length_type == 127) {
        // followed 8 bytes is payload length
        if(buf->readableBytes() < static_cast<int>(DataFrameInfo::MIN_HEADER_LENGTH) + 8) {
            return 0;
        }
        auto temp = *reinterpret_cast<const uint64_t*>(buf->peek() + cur_head_len);
        payload_length_ = muduo::net::sockets::networkToHost64(temp);    // big endian
        cur_head_len += 8;
    } else {
        payload_length_ = length_type;
    }

    if(mask_ == 1) {
        // masking key 4 bytes
        auto ini = buf->peek() + cur_head_len;
        std::copy(ini, ini + sizeof(masking_key_), masking_key_);
        cur_head_len += 4;
    }

    return cur_head_len;
}

uint64_t DataFrameCodec::parsePayload(const muduo::net::Buffer *buf)
{
    if(buf->readableBytes() < payload_length_) {
        return 0;
    }
    if(mask_ == 1) {
        LOG_DEBUG << "mask mode";
        for(uint64_t i = 0; i < payload_length_; ++i) {
            uint8_t temp = buf->peek()[i];
            temp ^= masking_key_[i % sizeof(masking_key_)];
            payload_.push_back(temp);
        }
    } else {
        payload_.append(buf->peek(), buf->peek() + payload_length_);
    }
    return payload_length_;
}
