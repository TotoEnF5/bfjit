#include <interpreter.h>
#include <iostream>
#include <stdexcept>
#include <string>

Interpreter::Interpreter(const std::string& code)
    : code(code)
{
    for (uint16_t i = 0; i < 0xFFFF; i++) {
	this->memory[i] = 0;
    }
}

void Interpreter::run() {
    while (this->programCounter < this->code.length()) {
	char operation = this->code.at(this->programCounter);

	switch (operation) {
	case '<':
	    this->decrementCellCounter();
	    break;

	case '>':
	    this->incrementCellCounter();
	    break;

	case '+':
	    this->incrementCell();
	    break;

	case '-':
	    this->decrementCell();
	    break;

	case '.':
	    this->output();
	    break;

	case ',':
	    this->input();
	    break;

	case '[':
	    this->jumpAfter();
	    break;

	case ']':
	    this->jumpBefore();
	    break;
	}

	this->programCounter++;
    }
}

void Interpreter::decrementCellCounter() {
    if (this->cellCounter == 0) {
	this->cellCounter = 0xFFFF;
    }
    else {
	this->cellCounter--;
    }
}


void Interpreter::incrementCellCounter() {
    if (this->cellCounter == 0xFFFF) {
	this->cellCounter = 0;
    }
    else {
	this->cellCounter++;
    }
}

void Interpreter::decrementCell() {
    if (this->memory.at(this->cellCounter) == 0) {
	this->memory.at(this->cellCounter) = 0xFFFF;
    }
    else {
	this->memory.at(this->cellCounter)--;
    }
}


void Interpreter::incrementCell() {
    if (this->memory.at(this->cellCounter) == 0xFFFF) {
	this->memory.at(this->cellCounter) = 0;
    }
    else {
	this->memory.at(this->cellCounter)++;
    }
}

void Interpreter::output() {
    std::cout << (char)this->memory.at(this->cellCounter);
}

void Interpreter::input() {
}

void Interpreter::jumpAfter() {
    if (this->memory.at(this->cellCounter) == 0) {
	size_t index = this->programCounter + 1;
	size_t numBrackets = 0;

	while (index < this->code.length()) {
	    if (this->code.at(index) == ']') {
		if (numBrackets == 0) {
		    this->programCounter = index;
		    return;
		}
		else if (numBrackets < 0) {
		    throw std::runtime_error(std::to_string(this->programCounter) + ": mismatched [");
		}
		else {
		    numBrackets--;
		}
	    }

	    if (this->code.at(index) == '[') {
		numBrackets++;
	    }

	    index++;
	}
    }
    else {
	return;
    }

    // if we get here, no matching ] was found
    throw std::runtime_error(std::to_string(this->programCounter) + ": no matching ] found");
}


void Interpreter::jumpBefore() {
    if (this->memory.at(this->cellCounter) != 0) {
	// jump to the first ]
	size_t index = this->programCounter - 1;
	size_t numBrackets = 0;

	while (index > 0) {
	    if (this->code.at(index) == '[') {
		if (numBrackets == 0) {
		    this->programCounter = index;
		    return;
		}
		else if (numBrackets < 0) {
		    throw std::runtime_error(std::to_string(this->programCounter) + ": mismatched ]");
		}
		else {
		    numBrackets--;
		}
	    }

	    if (this->code.at(index) == ']') {
		numBrackets++;
	    }

	    index--;
	}
    }
    else {
	return;
    }

    // if we get here, the matching ] was not found
    throw std::runtime_error(std::to_string(this->programCounter) + ": no matching [ found");
}
