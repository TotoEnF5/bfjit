#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "analyzer.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

class Interpreter {
public:
    Interpreter(const std::string& code);

    void run();

private:
    void decrementCellCounter();
    void incrementCellCounter();
    void decrementCell();
    void incrementCell();
    void output();
    void input();
    void jumpAfter();
    void jumpBefore();

private:
    Analyzer analyzer;
    std::string code;
    std::array<uint16_t, 0x10000> memory;
    uint16_t cellCounter = 0;
    size_t programCounter = 0;
};

#endif
