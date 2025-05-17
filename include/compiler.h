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
    void loadCellValue();
    void applyState();
    void output();
    void input();

private:
    jit_state_t* state;
    const uint8_t* memory;

    int32_t cellPointerState = 0;
    int32_t cellValueState = 0;

    char previousInstruction;
};

#endif
