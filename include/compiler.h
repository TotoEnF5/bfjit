#ifndef COMPILER_H_
#define COMPILER_H_

#include <cstdint>
#include <string>

extern "C" {
#include <lightning.h>
}

using CompiledFunction = void(*)(uint16_t cellPointer);

class Compiler {
public:
    Compiler(const uint8_t* memory);
    ~Compiler();

    CompiledFunction compile(const std::string& code);

private:
    void incrementCellPointer();
    void decrementCellPointer();
    void incrementCell();
    void decrementCell();

private:
    jit_state_t* state;
};

#endif
