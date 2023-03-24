//
// Created by clay on 3/24/23.
//

#ifndef WEBSOCKET_SERVER_HTTPPARSER_H
#define WEBSOCKET_SERVER_HTTPPARSER_H

#include <string_view>
#include <string>
#include <map>
#include <utility>
#include <vector>

class HttpParser {
    typedef std::map<std::string, std::string> ParaMap;
    static constexpr char EOL[] = "\r\n";
    static constexpr char SP[] = " ";
    static constexpr char COLON[] = ":";
public:
    HttpParser() = default;
    explicit HttpParser(std::string msg)
        : message_(std::move(msg))
    {}

    ParaMap::size_type parse();
    void setMessage(std::string msg) { message_ = std::move(msg); }
    [[nodiscard]] std::string_view getParam(const std::string& key) const { return param_.count(key) ? param_.at(key) : ""; }
private:
    std::string message_;
    std::string method_;
    std::string uri_;
    std::string version_;
    ParaMap param_;
};


#endif //WEBSOCKET_SERVER_HTTPPARSER_H
