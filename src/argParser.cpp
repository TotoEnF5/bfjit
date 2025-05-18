#include "argParser.h"
#include <algorithm>

ArgParser::ArgParser(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        this->tokens.push_back(argv[i]);
    }
}

const std::string& ArgParser::getOption(const std::string& option) {
    auto it = std::find(this->tokens.begin(), this->tokens.end(), option);
    if (it != this->tokens.end() && ++it != this->tokens.end()) {
        return *it;
    }
    
    static const std::string empty = "";
    return empty;
}

bool ArgParser::optionExists(const std::string& option) {
    return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
}
