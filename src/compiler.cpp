#include <compiler.h>
#include <lightning.h>
#include <lightning/jit_x86.h>
#include <stdio.h>

#define _jit this->state

Compiler::Compiler(const uint8_t* memory)
    : memory(memory)
{
    init_jit(nullptr);
    this->state = jit_new_state();
}

Compiler::~Compiler() {
    jit_destroy_state();
    finish_jit();
}

CompiledFunction Compiler::compile(const std::string& code) {
    this->previousInstruction = code.at(0);

    jit_prolog();

    // V0 = memory address
    jit_movi(JIT_V0, (jit_word_t)this->memory);
    // V1 = cell pointer, passed as argument to the function
    // calculate the offset in bytes, and store it in V1
    jit_node_t* arg = jit_arg();
    jit_getarg(JIT_V1, arg);
    jit_muli(JIT_V1, JIT_V1, sizeof(uint8_t));

    for (char instruction : code) {
	if (this->previousInstruction != instruction) {
	    this->applyState();
	}
	this->previousInstruction = instruction;

	switch (instruction) {
	case '<':
	    this->cellPointerState--;
	    break;

	case '>':
	    this->cellPointerState++;
	    break;

	case '+':
	    this->cellValueState++;
	    break;

	case '-':
	    this->cellValueState--;
	    break;

	case '.':
	    this->output();
	    break;

	case ',':
	    this->input();
	    break;
	}
    }
    
    jit_ret();
    CompiledFunction func = (CompiledFunction)jit_emit();
    jit_clear_state();

    return func;
}

void Compiler::loadCellValue() {
    // V2 = *(V0 + V1)
    jit_ldxr(JIT_V2, JIT_V0, JIT_V1);
}

void Compiler::applyState() {
    // cell pointer state
    if (this->cellPointerState > 0) {
	jit_addi(JIT_V1, JIT_V1, this->cellPointerState);
	this->cellPointerState = 0;
    }
    else if (this->cellPointerState < 0) {
	jit_subi(JIT_V1, JIT_V1, this->cellPointerState);
	this->cellPointerState = 0;
    }

    // cell value state
    if (this->cellValueState > 0) {
	this->loadCellValue();
	jit_addi(JIT_V2, JIT_V2, this->cellValueState);
	jit_stxr(JIT_V0, JIT_V1, JIT_V2);
	this->cellValueState = 0;
    }
    else if (this->cellValueState < 0) {
	this->loadCellValue();
	jit_subi(JIT_V2, JIT_V2, this->cellValueState);
	jit_stxr(JIT_V0, JIT_V1, JIT_V2);
	this->cellValueState = 0;
    }
}

void Compiler::output() {
    this->loadCellValue();

    jit_prepare();
    jit_pushargr_c(JIT_V2);
    jit_finishi((jit_pointer_t)putchar);
}

void Compiler::input() {
}
