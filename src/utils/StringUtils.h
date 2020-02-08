//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_STRINGUTILS_H
#define PROXIMA_STRINGUTILS_H

#include <string>
#include <iterator>
#include <algorithm>

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ') {
    std::size_t current, previous = 0;
    current = str.find(delim);
    while(current != std::string::npos) {
        cont.push_back(str.substr(previous, current-previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

#endif //PROXIMA_STRINGUTILS_H
