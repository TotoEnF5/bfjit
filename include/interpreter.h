#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "analyzer.h"
#include "cache.h"
#include "compiler.h"
#include <array>
#include <cstdint>
#include <map>
#include <string>

/**
 * The interpreter.
 */
class Interpreter {
public:
    /**
     * Initializes the memory and everything.
     * Pass in the brainfuck code you want to interpret.
     */
    Interpreter(const std::string& code, bool compile = true);

    /**
     * Run the brainfuck program!
     */
    void run();

    /**
     * Dumps the contents of the memory to stdout.
     */
    void dumpMemory();

private:
    /**
     * Moves the program counter to the matching ].
     */
    void gotoMatchingLoopEnd();

    /**
     * Moves the program counter to the matching [.
     */
    void gotoMatchingLoopBegin();

private:
    std::array<uint8_t, 0x10000> memory = { 0 };
    std::string code;

    Analyzer analyzer;
    Cache cache;
    Compiler compiler;

    bool compile;
    size_t programCounter = 0;
    uint16_t cellPointer = 0;

    std::map<size_t, size_t> loopBeginEnds;
    std::map<size_t, size_t> loopEndBegins;
};

#endif
