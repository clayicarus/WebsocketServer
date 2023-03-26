//
// Created by clay on 3/24/23.
//

#ifndef WEBSOCKET_SERVER_STRINGEXTRA_H
#define WEBSOCKET_SERVER_STRINGEXTRA_H

#include <string>
#include <string_view>
#include <cassert>

class StringExtra {
public:
    // split the string to list
    template<typename Container>
    static typename Container::size_type
    splitString(std::string_view s, std::string_view delimiter, Container &destination)
    {
        if(s.empty()) return 0;
        if(delimiter.empty()) {
            destination.clear();
            destination.emplace_back(s);
            return 1;
        }
        typename Container::size_type cnt = 0;
        std::string_view::size_type ini = 0;
        auto d = s.find_first_of(delimiter, ini);
        while(d != std::string_view::npos) {
            destination.emplace_back(s.substr(ini, d - ini));
            ++cnt;
            ini = d + delimiter.size();
            d = s.find_first_of(delimiter, ini);
        }
        destination.emplace_back(s.substr(ini));
        ++cnt;
        assert(cnt == destination.size());

        return cnt;
    }
    // trim character in the string
    static std::string &trim(std::string &s, const char chr = ' ')
    {
        if(s.empty()) return s;
        auto p = s.find_first_not_of(chr);
        if(p != std::string::npos) {
            s.erase(0, p);
        }
        p = s.find_last_of(chr);
        if(p != std::string::npos) {
            s.erase(s.find_last_not_of(chr) + 1);
        }
        return s;
    }
};

#endif //WEBSOCKET_SERVER_STRINGEXTRA_H
