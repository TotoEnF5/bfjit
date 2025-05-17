#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "analyzer.h"
#include "cache.h"
#include "compiler.h"
#include <array>
#include <cstdint>
#include <string>

class Interpreter {
public:
    Interpreter(const std::string& code);

    void run();
    void dumpMemory();

private:
    void gotoMatchingLoopEnd();
    void gotoMatchingLoopBegin();

private:
    std::array<uint8_t, 0x10000> memory = { 0 };
    std::string code;

    Analyzer analyzer;
    Cache cache;
    Compiler compiler;

    size_t programCounter = 0;
    uint16_t cellPointer = 0;
};

#endif
