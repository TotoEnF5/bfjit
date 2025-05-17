#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_

#include <vector>
#include <string>

/**
 * Mostly inspired by https://stackoverflow.com/a/868894
 */
class ArgParser {
public:
    ArgParser(int argc, char* argv[]);

    const std::string& getOption(const std::string& option);
    bool optionExists(const std::string& option);

private:
    std::vector<std::string> tokens;
};

#endif
