#include <compiler.h>
#include <lightning.h>

#define _jit this->state

Compiler::Compiler(const uint8_t* memory) {
    init_jit(nullptr);
    this->state = jit_new_state();
}

Compiler::~Compiler() {
    jit_destroy_state();
    finish_jit();
}

CompiledFunction Compiler::compile(const std::string& code) {
    jit_prolog();

    // compile stuff
    
    jit_ret();
    CompiledFunction func = (CompiledFunction)jit_emit();
    jit_clear_state();

    return func;
}
