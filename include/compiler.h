#ifndef COMPILER_H_
#define COMPILER_H_

#include <cstdint>
#include <string>
#include <vector>

extern "C" {
#include <lightning.h>
}

/**
 * A type for function pointers created by the compiler.
 */
using CompiledFunction = uint16_t(*)(uint16_t cellPointer);

/**
 * The compiler.
 * The memory location is in register V0.
 * The cell pointer is in register V1.
 * When loading the cell value, the value is set in register V2.
 * Other registers are used for everything else.
 */
class Compiler {
public:
    /**
     * Initializes the JIT.
     */
    Compiler(const uint8_t* memory);
    ~Compiler();

    /**
     * Compiles a sample of code.
     * The function will modify the memory of the interpreter, and return the new value of the cell pointer.
     * Returns the compiled function.
     */
    CompiledFunction compile(const std::string& code);

private:
    /**
     * Initializes lightning for compilation and sets the registers to the correct values.
     */
    void beginCompilation();

    /**
     * Ends lightning's compilation process.
     */
    CompiledFunction endCompilation();

    /**
     * Loads the value of the current cell into V2.
     */
    void loadCellValue();

    /**
     * Applies the state of the cell pointer and cell value.
     * As an optimisation, instead of incrementing the cell pointer and value every instruction,
     * we remember how the pointer/value will be changed.
     * For example, instead of doing 8 separate add instruction for '++++++++', we add 8 to the cell value only once.
     * This must be called everytime we get to a different inctruction from what we had before.
     */
    void applyState();

    /**
     * Inserts an output instruction.
     */
    void output();

    /**
     * Inserts an input instruction (does nothing for now).
     */
    void input();

    /**
     * Inserts the beginning of a loop.
     */
    void jumpForwards();

    /**
     * Inserts the end of a loop.
     */
    void jumpBackwards();

private:
    std::vector<jit_state_t*> states;
    uint32_t currentState = 0;

    const uint8_t* memory;

    jit_node_t* forwardLabels[1024] = { nullptr };
    jit_node_t* backwardLabels[1024] = { nullptr };
    uint32_t currentLoop = 0;

    int32_t cellPointerState = 0;
    int32_t cellValueState = 0;

    char previousInstruction;
};

#endif
