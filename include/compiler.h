#ifndef COMPILER_H_
#define COMPILER_H_

#include <cstdint>

extern "C" {
#include <lightning.h>
}

class Compiler {
public:
    Compiler(const uint8_t* memory);
    ~Compiler();

private:
    jit_state_t* state;
};

#endif
