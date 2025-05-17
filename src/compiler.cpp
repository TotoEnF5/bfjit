#include <compiler.h>
#include <lightning.h>
#include <lightning/jit_x86.h>
#include <stdio.h>

#define _jit this->states[this->currentState]

Compiler::Compiler(const uint8_t* memory)
    : memory(memory)
{
    init_jit(nullptr);
}

Compiler::~Compiler() {
    for (uint32_t i = 0; i < this->states.size(); i++) {
	this->currentState = i;
	jit_destroy_state();
    }

    finish_jit();
}

CompiledFunction Compiler::compile(const std::string& code) {
    this->states.push_back(jit_new_state());

    this->previousInstruction = code.at(0);

    this->beginCompilation();

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

	case '[':
	    this->jumpForwards();
	    break;

	case ']':
	    this->jumpBackwards();
	    break;
	}
    }

    this->applyState();
    
    CompiledFunction func = this->endCompilation();
    this->currentState++;
    return func;
}

void Compiler::beginCompilation() {
    jit_prolog();

    // V0 = memory address
    jit_movi(JIT_V0, (jit_word_t)this->memory);
    // V1 = cell pointer, passed as argument to the function
    jit_node_t* arg = jit_arg();
    jit_getarg(JIT_V1, arg);
}

CompiledFunction Compiler::endCompilation() {
    jit_retr(JIT_V1);
    CompiledFunction func = (CompiledFunction)jit_emit();
    jit_clear_state();
    return func;
}

void Compiler::loadCellValue() {
    // V2 = *(V0 + V1)
    jit_ldxr_uc(JIT_V2, JIT_V0, JIT_V1);
}

void Compiler::applyState() {
    // cell pointer state
    if (this->cellPointerState > 0) {
	jit_addi(JIT_V1, JIT_V1, this->cellPointerState);
	this->cellPointerState = 0;
    }
    else if (this->cellPointerState < 0) {
	// if the state is negative, we tell lightning to do a subtraction operation
	// but since the state is negative, we need to negate it or we subtract by something stupid like 0xFFFFFFFFF
	// (the representation of a negative number in binary)
	jit_subi(JIT_V1, JIT_V1, -this->cellPointerState);
	this->cellPointerState = 0;
    }

    // cell value state
    if (this->cellValueState > 0) {
	this->loadCellValue();
	jit_addi(JIT_V2, JIT_V2, this->cellValueState);
	jit_stxr_c(JIT_V0, JIT_V1, JIT_V2);
	this->cellValueState = 0;
    }
    else if (this->cellValueState < 0) {
	// if the state is negative, we tell lightning to do a subtraction operation
	// but since the state is negative, we need to negate it or we subtract by something stupid like 0xFFFFFFFFF
	// (the representation of a negative number in binary)
	this->loadCellValue();
	jit_subi(JIT_V2, JIT_V2, -this->cellValueState);
	jit_stxr_c(JIT_V0, JIT_V1, JIT_V2);
	this->cellValueState = 0;
    }
}

void Compiler::output() {
    this->loadCellValue();

    // see https://www.gnu.org/software/lightning/manual/html_node/printf.html
    jit_prepare();
    jit_pushargr_c(JIT_V2);
    jit_finishi((jit_pointer_t)putchar);
}

void Compiler::input() {
}

void Compiler::jumpForwards() {
    this->loadCellValue();

    // see www.gnu.org/software/lightning/manual/html_node/The-instruction-set.html, "Labels" section
    jit_node_t* forward = jit_forward();
    jit_node_t* jump = jit_beqi(JIT_V2, 0); // if V2 == 0, jmp to corresponding ]
    jit_patch_at(jump, forward);

    jit_node_t* label = jit_label(); // label to jump to for the corresponding ]
    
    this->forwardLabels[this->currentLoop] = forward;
    this->backwardLabels[this->currentLoop] = label;

    this->currentLoop++;
}

void Compiler::jumpBackwards() {
    this->loadCellValue();

    this->currentLoop--;

    // see www.gnu.org/software/lightning/manual/html_node/The-instruction-set.html, "Labels" section
    jit_node_t* backward = jit_bnei(JIT_V2, 0); // if V2 != 0, jmp to [
    jit_patch_at(backward, this->backwardLabels[this->currentLoop]);
    
    jit_link(this->forwardLabels[this->currentLoop]);
}
