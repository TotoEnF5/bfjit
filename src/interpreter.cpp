#include "interpreter.h"
#include "analyzer.h"
#include "compiler.h"
#include <iostream>
#include <stdexcept>
#include <string>

Interpreter::Interpreter(const std::string& code)
    : code(code), analyzer(3), compiler(this->memory.data())
{}

void Interpreter::run() {
    while (this->programCounter < this->code.length()) {
	char instruction = this->code.at(this->programCounter);
	switch (instruction) {
	case '<':
	    this->cellPointer--;
	    break;

	case '>':
	    this->cellPointer++;
	    break;

	case '+':
	    this->memory.at(this->cellPointer)++;
	    break;

	case '-':
	    this->memory.at(this->cellPointer)--;
	    break;

	case '.':
	    std::cout << (char)this->memory.at(this->cellPointer);
	    break;

	case '[':
	    if (this->memory.at(this->cellPointer) == 0) {
		this->gotoMatchingLoopEnd();
	    }
	    else {
		if (this->cache.isInCache(this->programCounter)) {
		    CompiledFunction func = this->cache.getFunction(this->programCounter);
		    this->cellPointer = func(this->cellPointer);

		    this->gotoMatchingLoopEnd();
		    this->programCounter--;
		}
		else if (this->analyzer.analyzeLoop(this->programCounter)) {
		    uint16_t oldCounter = this->programCounter;
		    this->gotoMatchingLoopEnd();
		    uint16_t matchingEnd = this->programCounter;
		    this->programCounter = oldCounter;

		    std::string codeToCompile = this->code.substr(this->programCounter + 1, matchingEnd - this->programCounter - 1);
		    CompiledFunction func = this->compiler.compile(codeToCompile);
		    this->cache.addFunction(this->programCounter, func);

		    this->cellPointer = func(this->cellPointer);
		    this->programCounter = matchingEnd - 1;
	    	}
		else {
		    // do nothing
		}
	    }
	    break;
	
	case ']':
	    if (this->memory.at(this->cellPointer) != 0) {
		this->gotoMatchingLoopBegin();
	    }
	    break;

	default:
	    break;
	}

	this->programCounter++;
    }
}

void Interpreter::dumpMemory() {
    std::cout << "=== MEMORY DUMP BEGIN ===" << std::endl;
    for (uint8_t b : this->memory) {
	std::cout << (uint32_t)b << std::endl;
    }
    std::cout << "=== MEMORY DUMP END ===" << std::endl;
}

void Interpreter::gotoMatchingLoopEnd() {
    uint16_t baseCounter = this->programCounter;
    uint32_t numLoops = 0;

    this->programCounter++;
    while (this->programCounter < this->code.length()) {
	char instruction = this->code.at(this->programCounter);
	if (instruction == '[') {
	    numLoops++;
	}
	else if (instruction == ']') {
	    if (numLoops == 0) {
		return;
	    }

	    numLoops--;
	}

	this->programCounter++;
    }

    throw std::runtime_error(std::to_string(baseCounter) + ": no matching ]");
}

void Interpreter::gotoMatchingLoopBegin() {
    uint16_t baseCounter = this->programCounter;
    uint32_t numLoops = 0;

    this->programCounter--;
    while (this->programCounter > 0) {
	char instruction = this->code.at(this->programCounter);
	if (instruction == ']') {
	    numLoops++;
	}
	else if (instruction == '[') {
	    if (numLoops == 0) {
		return;
	    }

	    numLoops--;
	}

	this->programCounter--;
    }

    throw std::runtime_error(std::to_string(baseCounter) + ": no matching [");
}
