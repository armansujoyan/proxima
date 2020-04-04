//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_STRINGUTILS_H
#define PROXIMA_STRINGUTILS_H

#include <string>
#include <iterator>
#include <algorithm>
#include <sstream>

template <class Container>
static void split(const std::string& str, Container& cont, char delim = ' ') {
    std::size_t current, previous = 0;
    current = str.find(delim);
    while(current != std::string::npos) {
        cont.push_back(str.substr(previous, current-previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

static glm::vec3 ParseVec3(const std::string &line) {
    std::stringstream ss(line);
    float parsedValues[3];

    for (float &parsedValue : parsedValues) {
        ss >> parsedValue;
    }

    return glm::vec3(
            parsedValues[0],
            parsedValues[1],
            parsedValues[2]);
}

static glm::vec2 ParseVec2(const std::string &line) {
    std::stringstream ss(line);
    float parsedValues[3];

    for (int i = 0; i < 2; ++i) {
        ss >> parsedValues[i];
    }

    return glm::vec2(
            parsedValues[0],
            parsedValues[1]);
}

static std::string getRelativePath(const std::string &rootPath, const std::string &fileName) {
    size_t lastSlashPosition = rootPath.find_last_of('/');
    std::string relativePath = rootPath.substr(0, lastSlashPosition + 1) + fileName;
    return relativePath;
}

#endif //PROXIMA_STRINGUTILS_H
