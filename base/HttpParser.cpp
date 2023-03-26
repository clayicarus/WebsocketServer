//
// Created by clay on 3/24/23.
//

#include <algorithm>
#include "HttpParser.h"
#include "StringExtra.h"

size_t HttpParser::parseOne(std::string_view one_request)
{
    const auto doubleEOL = std::string(EOL) + EOL;
    auto poz = one_request.find(doubleEOL);
    if(poz == std::string::npos) {
        return 0;
    }
    // split message
    std::vector<std::string> lines;
    if(StringExtra::splitString(one_request.substr(0, poz), EOL, lines) < 2) {
        return 0;
    }
    // trim all lines
    for(auto &i : lines) { StringExtra::trim(i); }
    // parse one request head
    std::vector<std::string> requestParams;
    if(StringExtra::splitString(lines.front(), SP, requestParams) < 3) {
        return 0;
    }
    method_ = StringExtra::trim(requestParams[0]);
    uri_ = StringExtra::trim(requestParams[1]);
    version_ = StringExtra::trim(requestParams[2]);
    // parseOne headers to params
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

    return poz + doubleEOL.size();
}

std::string_view HttpParser::getParam(const std::string &key) const
{
    auto p = param_.find(key);
    if(p != param_.end()) return p->second;
    else return {};
}
