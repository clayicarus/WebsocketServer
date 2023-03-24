//
// Created by clay on 3/24/23.
//

#include <algorithm>
#include "HttpParser.h"
#include "StringExtra.h"

HttpParser::ParaMap::size_type HttpParser::parse()
{
    auto e = message_.find(std::string(EOL) + EOL);
    if(e == std::string::npos) {
        return -1;
    }
    // split message
    std::vector<std::string> lines;
    if(StringExtra::splitString(message_.substr(0, e), EOL, lines) < 2) {
        return -1;
    }
    // trim all lines
    for(auto &i : lines) { StringExtra::trim(i); }
    // parse request head
    std::vector<std::string> requestParams;
    if(StringExtra::splitString(lines.front(), SP, requestParams) < 3) {
        return -1;
    }
    method_ = StringExtra::trim(requestParams[0]);
    uri_ = StringExtra::trim(requestParams[1]);
    version_ = StringExtra::trim(requestParams[2]);
    // parse headers to params
    for(int i = 1; i < lines.size(); ++i) {
        const auto &l = lines[i];
        auto pos = l.find_first_of(COLON);
        if(pos == std::string::npos) continue;
        auto key = l.substr(0, pos);
        auto value = l.substr(pos + 1);
        if(StringExtra::trim(key).empty()) continue;
        if(StringExtra::trim(value).empty()) continue;
        param_.emplace(key, value);
    }

    return param_.size();
}