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
public:
    static constexpr char SP[] = " ";
    static constexpr char COLON[] = ":";
    static constexpr char EOL[] = "\r\n";

    HttpParser() = default;

    // return parse size
    size_t parseOne(std::string_view one_request);
    [[nodiscard]] std::string_view getParam(const std::string& key) const;
    [[nodiscard]] std::string_view getMethod() const { return method_; }
    [[nodiscard]] std::string_view getUri() const { return uri_; };
    [[nodiscard]] std::string_view getVersion() const { return version_; }
    [[nodiscard]] const ParaMap &params() const { return param_; }
private:
    std::string message_;
    std::string method_;
    std::string uri_;
    std::string version_;
    ParaMap param_;
};


#endif //WEBSOCKET_SERVER_HTTPPARSER_H
